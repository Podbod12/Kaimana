// Kaimana preview simulator.
//
// A JavaScript port of the firmware's LED logic (idle modes, button press colours and fades, special move detection
// and the combo animations) so the character editor can show what a character looks like before you flash it.
// It follows animations.cpp, kaimana.cpp and the main loop in kaimana-zero.ino closely, including their timings,
// so if you change those files you may want to change the matching code here too.

(function (root) {
  'use strict';

  const cg = root.KaimanaCodegen || (typeof require !== 'undefined' ? require('./codegen.js') : null);

  const NONE = 0xFF;

  // colorCycleData from kaimana_custom.h: 320 zeros, 64 step ramp up, 320 x 255, 64 step ramp down
  const RAMP = [];
  for (let i = 0; i < 16; ++i) RAMP.push(i * 2);
  for (let i = 0; i < 16; ++i) RAMP.push(32 + i * 3);
  for (let i = 0; i < 16; ++i) RAMP.push(81 + i * 4);
  for (let i = 0; i < 16; ++i) RAMP.push(148 + i * 7);
  const COLOR_CYCLE = [].concat(new Array(320).fill(0), RAMP, new Array(320).fill(255), RAMP.slice().reverse());

  const IDLE_SIZE = 768, IDLE_OFFSET_2 = 512, IDLE_OFFSET_1 = 256, IDLE_OFFSET_0 = 0, IDLE_OFFSET = 12;
  const FIREBALL_SIZE = 768, FIREBALL_OFFSET_1 = 96, FIREBALL_OFFSET_2 = 192, FIREBALL_OFFSET_3 = 288;
  const FIREBALL_DELAY_MS = 0.35, FIREBALL_LOOP_START = 512, FIREBALL_LOOP_FRAMES = 64, FIREBALL_LOOP_STROBE_LOW = 0.5;
  const FIREBALL_SPEEDS = { Slow: 2, Medium: 4, Fast: 6 };
  const MIN_LED_UPDATE_DELAY = 5;
  const FLASH_TIMINGS = [100, 80, 60, 40, 20, 10, 5];

  const COMBO_INPUT_TIME_WINDOW = 200, COMBO_TRIGGER_INPUT_TIME_WINDOW = 125, CHARGE_COMBO_INPUT_TIME_WINDOW = 750;
  const COMBO_INPUT_COUNT_FOR_ONE_OUT_OF_TWO = 7, SWITCH_HISTORY_MAX = 24;

  const ATTACKS = ['P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4'];
  const LED_NAMES = ['P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4', 'Up', 'Down', 'Left', 'Right', 'Select', 'Start', 'Home'];

  // Default settings, matching kaimana_custom.h as shipped
  const DEFAULT_CONFIG = {
    leds: { P4: 12, P3: 8, P2: 4, P1: 0, K1: 28, K2: 24, K3: 20, K4: 16, Home: NONE, Start: NONE, Select: NONE, Up: 44, Down: 36, Left: 32, Right: 40 },
    ledCount: 48,
    perButton: 4,
    perDirection: 4,
    ledList: ['Up', 'Right', 'P1', 'P2', 'P3', 'P4', 'K4', 'K3', 'K2', 'K1', 'Down', 'Left'],
    idleTimeoutMs: 1000,
    idlePulseSpeed: 0.002,
    circlePulseSpeed: 8,
  };

  const u32 = x => x >>> 0;
  const sub = (a, b) => (a - b) >>> 0; // unsigned long subtraction, like the firmware

  // Reads the LED layout from kaimana_custom.h so the preview matches the user's stick. Anything missing keeps the default.
  function parseConfig(text) {
    const cfg = JSON.parse(JSON.stringify(DEFAULT_CONFIG));
    if (!text) return cfg;
    const src = text.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/[^\n]*/g, '');
    const defs = {};
    const re = /#define\s+(\w+)\s+([^\s]+)/g;
    let m;
    while ((m = re.exec(src))) defs[m[1]] = m[2];
    const num = v => (v === undefined ? undefined : (/^0x/i.test(v) ? parseInt(v, 16) : (/^\d+$/.test(v) ? parseInt(v, 10) : undefined)));
    const names = { P1: 'LED_P1', P2: 'LED_P2', P3: 'LED_P3', P4: 'LED_P4', K1: 'LED_K1', K2: 'LED_K2', K3: 'LED_K3', K4: 'LED_K4', Home: 'LED_HOME', Start: 'LED_START', Select: 'LED_SELECT', Up: 'LED_UP', Down: 'LED_DOWN', Left: 'LED_LEFT', Right: 'LED_RIGHT' };
    for (const k in names) { const v = num(defs[names[k]]); if (v !== undefined) cfg.leds[k] = v; }
    const lc = num(defs.LED_COUNT); if (lc) cfg.ledCount = lc;
    const pb = num(defs.LED_PER_BUTTON); if (pb) cfg.perButton = pb;
    const pd = num(defs.LED_PER_JOYSTICK_DIRECTION); if (pd) cfg.perDirection = pd;
    const it = num(defs.IDLE_TIMEOUT_SECONDS); if (it !== undefined) cfg.idleTimeoutMs = it * 1000;
    const ps = parseFloat(defs.IDLE_PULSE_SPEED); if (!isNaN(ps)) cfg.idlePulseSpeed = ps;
    const cs = num(defs.IDLE_CIRCLEPULSE_SPEED); if (cs) cfg.circlePulseSpeed = cs;
    const list = src.match(/ledList\s*\[[^\]]*\]\s*=\s*\{([^}]*)\}/);
    if (list) {
      const byLed = {};
      for (const k in names) byLed[names[k]] = k;
      const parsed = list[1].split(',').map(s => byLed[s.trim()]).filter(Boolean);
      if (parsed.length) cfg.ledList = parsed;
    }
    return cfg;
  }

  class AbortAnimation extends Error {}

  class KaimanaSim {
    constructor(config) {
      this.cfg = config || JSON.parse(JSON.stringify(DEFAULT_CONFIG));
      this.character = null;
      this.motions = {};
      this.held = new Set();
      this.onMove = null;     // called with the move when a special move is detected
      this.onRender = null;   // called with the displayed LED colours after each update
      this.speed = 1;
      this.reset();
    }

    setConfig(config) { this.cfg = config; this.reset(); }

    reset() {
      const n = this.cfg.ledCount;
      this.led = Array.from({ length: n }, () => [0, 0, 0]);
      this.display = this.led.map(c => c.slice());
      this.iLED = new Array(n).fill(false);
      this.blends = this.cfg.ledList.map(name => ({ pin: this.cfg.leds[name], timeSet: 0, hold: 0, fade: 0, src: [0, 0, 0], dst: [0, 0, 0] }));
      this.history = Array.from({ length: SWITCH_HISTORY_MAX }, () => ({ input: null, timeSet: 0, timeReleased: 0, held: false }));
      this.inputsThisFrame = new Set();
      this.awaitingRelease = false;
      this.frameStart = 0;
      this.frameIndex = 0;
      this.workingIndex = 0;
      this.idleTimeout = 0;
      this.joystickLast = null;
    }

    setCharacter(character, motions) {
      this.character = character;
      this.motions = motions || {};
      this.idleTimeout = 0; // go straight to idle like a fresh boot
      this.frameIndex = 0;
    }

    // ---- time ----
    // Firmware delays are blocking. We run on a virtual clock that only waits for real time when it gets ahead,
    // so animations made of thousands of tiny delays still play at the right speed.
    realNow() { return (typeof performance !== 'undefined' ? performance.now() : Date.now()) * this.speed; }
    millis() { return Math.floor(this.vnow); }
    async sleep(ms) {
      if (this.aborting && this.inAnimation) throw new AbortAnimation();
      this.vnow += ms;
      const lag = this.realNow() - this.vnow;
      if (lag > 250) this.vnow += lag; // tab was hidden, dont try to catch up
      while (this.vnow > this.realNow()) {
        await new Promise(r => setTimeout(r, Math.min(16, Math.max(1, (this.vnow - this.realNow()) / this.speed))));
        if (this.aborting && this.inAnimation) throw new AbortAnimation();
        if (!this.running) throw new AbortAnimation();
      }
    }
    updateMs() { return this.cfg.ledCount * 24 * 0.00125; } // WS2811 data time for updateALL

    // ---- kaimana.cpp ----
    ledCountFor(index) {
      const l = this.cfg.leds;
      return (index === l.Up || index === l.Down || index === l.Left || index === l.Right) ? this.cfg.perDirection : this.cfg.perButton;
    }
    setIndividualLED(i, r, g, b) {
      if (i === NONE || i < 0 || i >= this.cfg.ledCount) return;
      this.led[i] = [r & 0xFF, g & 0xFF, b & 0xFF];
    }
    setLED(index, r, g, b, isBlend, holdTime, fadeTime) {
      if (index === NONE || index === undefined) return;
      const bi = this.blends.findIndex(x => x.pin === index);
      if (bi >= 0 && !isBlend) {
        const bl = this.blends[bi];
        if ((holdTime || 0) > 0 || (fadeTime || 0) > 0) {
          if (bl.timeSet === 0 || bl.dst[0] !== (r & 0xFF) || bl.dst[1] !== (g & 0xFF) || bl.dst[2] !== (b & 0xFF)) {
            bl.src = (this.led[index] || [0, 0, 0]).slice();
            bl.dst = [r & 0xFF, g & 0xFF, b & 0xFF];
            bl.timeSet = this.millis() || 1;
            bl.hold = holdTime || 0;
            bl.fade = fadeTime || 0;
          }
          return;
        }
        bl.timeSet = 0;
      }
      for (let k = 0; k < this.ledCountFor(index); ++k) this.setIndividualLED(index + k, r, g, b);
    }
    setALL(r, g, b) {
      this.blends.forEach(bl => { bl.timeSet = 0; });
      for (let i = 0; i < this.cfg.ledCount; ++i) this.setIndividualLED(i, r, g, b);
      this.updateALL();
    }
    blendLEDs(forceEnd) {
      const now = this.millis();
      let going = false;
      for (const bl of this.blends) {
        if (bl.timeSet === 0) continue;
        going = true;
        const start = bl.timeSet + bl.hold;
        if (forceEnd || now > start) {
          let p = bl.fade > 0 ? (now - start) / bl.fade : 1;
          if (forceEnd) p = 1;
          if (p >= 1) { p = 1; bl.timeSet = 0; }
          this.setLED(bl.pin, Math.trunc(bl.src[0] + (bl.dst[0] - bl.src[0]) * p), Math.trunc(bl.src[1] + (bl.dst[1] - bl.src[1]) * p), Math.trunc(bl.src[2] + (bl.dst[2] - bl.src[2]) * p), true);
        }
      }
      return going;
    }
    updateALL() {
      this.display = this.led.map(c => c.slice());
      if (this.onRender) this.onRender(this.display);
    }

    switchHistoryClear() {
      this.awaitingRelease = true;
      this.history.forEach(h => { h.input = null; });
    }
    switchHistoryBeginFrame() { this.frameStart = this.millis(); this.inputsThisFrame.clear(); }
    switchHistoryEndFrame() {
      if (this.inputsThisFrame.size === 0) this.awaitingRelease = false;
      for (const h of this.history) {
        if (h.held && !this.inputsThisFrame.has(h.input)) { h.held = false; h.timeReleased = this.frameStart; }
      }
    }
    switchHistorySet(input) {
      this.inputsThisFrame.add(input);
      if (this.awaitingRelease) return;
      let needNew = true;
      for (const h of this.history) {
        if (h.input === input) { if (h.held) needNew = false; break; }
      }
      if (needNew) {
        this.history.pop();
        this.history.unshift({ input, timeSet: this.millis(), timeReleased: 0, held: true });
      }
    }
    static inputMatches(historyInput, comboInput, charge) {
      if (historyInput === comboInput) return true;
      if (!charge) return false;
      if (comboInput === 'Left') return historyInput === 'UpLeft' || historyInput === 'DownLeft';
      if (comboInput === 'Down') return historyInput === 'DownLeft' || historyInput === 'DownRight';
      if (comboInput === 'Right') return historyInput === 'UpRight' || historyInput === 'DownRight';
      if (comboInput === 'Up') return historyInput === 'UpLeft' || historyInput === 'UpRight';
      return false;
    }
    switchHistoryTest(move, triggers, charge) {
      let lastFound = -1, doneTriggers = false, missed = 0;
      const now = u32(this.millis());
      let timeLast = now;
      const allowMiss = move.length >= COMBO_INPUT_COUNT_FOR_ONE_OUT_OF_TWO;
      const combo = move.concat(triggers).slice(0, SWITCH_HISTORY_MAX);
      const H = this.history;
      for (let ci = combo.length - 1; ci >= 0; --ci) {
        if (!doneTriggers) {
          if (!ATTACKS.includes(combo[ci])) doneTriggers = true;
          let found = false;
          for (let hi = 0; hi < SWITCH_HISTORY_MAX; ++hi) {
            if (sub(now, H[hi].timeSet) > COMBO_TRIGGER_INPUT_TIME_WINDOW) return false;
            if (KaimanaSim.inputMatches(H[hi].input, combo[ci], charge)) { found = true; break; }
          }
          if (!found) return false;
          continue;
        }
        let found = false;
        for (let hi = lastFound + 1; hi < SWITCH_HISTORY_MAX; ++hi) {
          const h = H[hi];
          if (!KaimanaSim.inputMatches(h.input, combo[ci], charge)) continue;
          found = true;
          if (ci === 0 && charge) {
            if (sub(timeLast, h.timeReleased) < COMBO_INPUT_TIME_WINDOW && sub(h.timeReleased, h.timeSet) > CHARGE_COMBO_INPUT_TIME_WINDOW) {
              this.switchHistoryClear();
              return true;
            }
            return false;
          }
          const t = h.held ? h.timeSet : h.timeReleased;
          if (sub(timeLast, t) > COMBO_INPUT_TIME_WINDOW) {
            if (!allowMiss) return false;
            if (++missed === 2) return false;
          }
          timeLast = u32(h.timeSet);
          lastFound = hi;
          missed = 0;
          break;
        }
        if (!found) {
          if (!allowMiss) return false;
          if (++missed === 2) return false;
        }
      }
      this.switchHistoryClear();
      this.blends.forEach(bl => { bl.timeSet = 0; });
      return true;
    }

    // ---- character colours ----
    colourFor(mapName, ledName) {
      const c = this.character;
      const table = {
        idleStatic: [c && c.idle && c.idle.staticColour, '#000000'],
        idlePulse: [c && c.idle && c.idle.pulseColour, '#ffffff'],
        notPressed: [c && c.notPressedColour, '#000000'],
        pressed: [c && c.pressedColour, 'random'],
      }[mapName];
      return this.rgb(cg.resolveColour(table[0], ledName, table[1]));
    }
    rgb(hex) {
      if (hex === 'random') hex = cg.RANDOM_COLOURS[Math.floor(Math.random() * cg.RANDOM_COLOURS.length)];
      return cg.hexToRgb(hex);
    }
    ledName(index) {
      for (const n of LED_NAMES) if (this.cfg.leds[n] === index) return n;
      return 'P1';
    }

    // ---- animations.cpp ----
    getBlendedPulseColour(i, ledIndex, frameIndex) {
      const offset = i === 0 ? IDLE_OFFSET_2 : (i === 1 ? IDLE_OFFSET_1 : IDLE_OFFSET_0);
      const name = this.cfg.ledList[ledIndex];
      const s = this.colourFor('idleStatic', name);
      const p = this.colourFor('idlePulse', name);
      const ci = frameIndex + offset;
      if (ci >= IDLE_SIZE) return s;
      const mul = COLOR_CYCLE[ci] / 256;
      return [0, 1, 2].map(k => (s[k] + Math.trunc((p[k] - s[k]) * mul)) & 0xFF);
    }
    setStaticColourToAllLeds() {
      for (const name of this.cfg.ledList) {
        const c = this.colourFor('idleStatic', name);
        this.setLED(this.cfg.leds[name], c[0], c[1], c[2]);
      }
    }
    animationIdle() {
      const type = (this.character && this.character.idle && this.character.idle.type) || 'RainbowCircling';
      const pulseTypes = ['StaticColourCirclePulse', 'StaticColourCircleDualPulse', 'StaticColourPingPongPulse'];
      const isPulse = pulseTypes.includes(type);
      const entries = this.cfg.ledList.length;
      let loop = false;
      this.frameIndex += isPulse ? this.cfg.circlePulseSpeed : 1;
      if (this.frameIndex >= (isPulse ? IDLE_OFFSET_1 : IDLE_SIZE)) { this.frameIndex = 0; loop = true; }
      const f = this.frameIndex;
      const n = this.cfg.ledCount;
      if (type === 'RainbowCircling') {
        for (let i = 0; i < n; ++i) {
          const o = (n - i) * IDLE_OFFSET;
          this.setIndividualLED(i, COLOR_CYCLE[(f + IDLE_OFFSET_2 + o) % IDLE_SIZE], COLOR_CYCLE[(f + IDLE_OFFSET_1 + o) % IDLE_SIZE], COLOR_CYCLE[(f + IDLE_OFFSET_0 + o) % IDLE_SIZE]);
        }
      } else if (type === 'RainbowPulsing') {
        this.setALL(COLOR_CYCLE[(f + IDLE_OFFSET_2) % IDLE_SIZE], COLOR_CYCLE[(f + IDLE_OFFSET_1) % IDLE_SIZE], COLOR_CYCLE[(f + IDLE_OFFSET_0) % IDLE_SIZE]);
      } else if (type === 'StaticColour') {
        this.setStaticColourToAllLeds();
      } else if (type === 'StaticColourPulsing') {
        const mul = 0.2 + (1 + Math.sin(this.cfg.idlePulseSpeed * this.millis())) * 0.4;
        for (const name of this.cfg.ledList) {
          const c = this.colourFor('idleStatic', name);
          this.setLED(this.cfg.leds[name], Math.trunc(c[0] * mul), Math.trunc(c[1] * mul), Math.trunc(c[2] * mul));
        }
      } else if (type === 'StaticColourCirclePulse' || type === 'StaticColourCircleDualPulse') {
        this.setStaticColourToAllLeds();
        if (this.workingIndex < 0 || this.workingIndex >= entries) this.workingIndex = 0;
        if (loop) this.workingIndex = (this.workingIndex + 1) % entries;
        for (let i = 0; i < 3; ++i) {
          const li = (i + this.workingIndex) % entries;
          const c = this.getBlendedPulseColour(i, li, f);
          this.setLED(this.cfg.leds[this.cfg.ledList[li]], c[0], c[1], c[2]);
          if (type === 'StaticColourCircleDualPulse') {
            const second = (li + Math.floor(entries / 2)) % entries;
            this.setLED(this.cfg.leds[this.cfg.ledList[second]], c[0], c[1], c[2]);
          }
        }
      } else if (type === 'StaticColourPingPongPulse') {
        this.setStaticColourToAllLeds();
        if (this.workingIndex < -2 || this.workingIndex > entries * 2 + 1) this.workingIndex = -2;
        if (loop) { this.workingIndex++; if (this.workingIndex > entries * 2 + 1) this.workingIndex = -2; }
        for (let i = 0; i < 3; ++i) {
          let li = this.workingIndex + i;
          if (this.workingIndex >= entries) li = ((entries * 2) - 1 - this.workingIndex) + (2 - i);
          if (li < 0 || li >= entries) continue;
          const c = this.getBlendedPulseColour(i, li, f);
          this.setLED(this.cfg.leds[this.cfg.ledList[li]], c[0], c[1], c[2]);
        }
      }
    }

    waveSetButton(counter, button, r, g, b) {
      if (counter < FIREBALL_SIZE && counter >= 0) {
        const v = COLOR_CYCLE[counter % FIREBALL_SIZE];
        this.setLED(button, Math.trunc(v * Math.trunc(r) / 255), Math.trunc(v * Math.trunc(g) / 255), Math.trunc(v * Math.trunc(b) / 255));
      } else {
        this.setLED(button, 0, 0, 0);
      }
    }
    async wave(direction, speed, loops, [r, g, b]) {
      const L = this.cfg.leds;
      const counter = [FIREBALL_SIZE - 1, FIREBALL_SIZE - 1 + FIREBALL_OFFSET_1, FIREBALL_SIZE - 1 + FIREBALL_OFFSET_2, FIREBALL_SIZE - 1 + FIREBALL_OFFSET_3];
      const loopsLeft = [loops, loops, loops, loops];
      const loopFrames = [0, 0, 0, 0];
      const mult = [1, 1, 1, 1];
      this.setALL(0, 0, 0);
      await this.sleep(MIN_LED_UPDATE_DELAY);
      let order;
      const punchOnly = /PunchOnly/.test(direction), kickOnly = /KickOnly/.test(direction);
      if (direction.startsWith('LeftToRight') || direction.startsWith('RightToLeft')) {
        const cols = direction.startsWith('LeftToRight') ? [['P1', 'K1'], ['P2', 'K2'], ['P3', 'K3'], ['P4', 'K4']] : [['P4', 'K4'], ['P3', 'K3'], ['P2', 'K2'], ['P1', 'K1']];
        order = [];
        cols.forEach(([p, k], gi) => {
          order.push({ button: L[p], group: kickOnly ? -1 : gi });
          order.push({ button: L[k], group: punchOnly ? -1 : gi });
        });
      } else {
        const g1 = direction === 'UpToDown' ? 1 : 0;
        order = ['K1', 'K2', 'K3', 'K4'].map(k => ({ button: L[k], group: g1 })).concat(['P1', 'P2', 'P3', 'P4'].map(p => ({ button: L[p], group: 1 - g1 })));
      }
      const step = FIREBALL_SPEEDS[speed] || 2;
      while (counter[0] >= 0) {
        for (let gi = 0; gi < 4; ++gi) {
          mult[gi] = 1;
          if (loopsLeft[gi] > 0 && counter[gi] < FIREBALL_LOOP_START) {
            loopFrames[gi]++;
            if (loopFrames[gi] >= FIREBALL_LOOP_FRAMES) { loopFrames[gi] = 0; loopsLeft[gi]--; }
            const rad = 6.2831 * (loopFrames[gi] / FIREBALL_LOOP_FRAMES);
            mult[gi] = FIREBALL_LOOP_STROBE_LOW + (1 - FIREBALL_LOOP_STROBE_LOW) * ((1 + Math.cos(rad)) / 2);
          }
        }
        for (const o of order) {
          if (o.group < 0 || o.group >= 4 || o.button === NONE) continue;
          this.waveSetButton(counter[o.group], o.button, r * mult[o.group], g * mult[o.group], b * mult[o.group]);
        }
        this.updateALL();
        await this.sleep(FIREBALL_DELAY_MS + this.updateMs());
        for (let gi = 0; gi < 4; ++gi) {
          if (loopsLeft[gi] <= 0 || counter[gi] > FIREBALL_LOOP_START) counter[gi] -= step;
        }
      }
      this.setALL(0, 0, 0);
      await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async flashColour([r, g, b], time) {
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
      this.setALL(r, g, b); await this.sleep(time);
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async singleCircle([r, g, b]) {
      for (const k of ['K1', 'K2', 'K3', 'K4', 'P4', 'P3', 'P2', 'P1']) {
        this.setLED(this.cfg.leds[k], r, g, b);
        this.updateALL();
        await this.sleep(25);
      }
    }
    async circleOneColour(loops, col) {
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
      for (; loops >= 0; --loops) { await this.singleCircle(col); await this.singleCircle([0, 0, 0]); }
      await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async circleRGB(loops) {
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
      for (; loops >= 0; --loops) { await this.singleCircle([255, 0, 0]); await this.singleCircle([0, 255, 0]); await this.singleCircle([0, 0, 255]); }
      await this.singleCircle([0, 0, 0]);
      await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async flashAllSpeedIncreasing([r, g, b]) {
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
      for (const t of FLASH_TIMINGS) { this.setALL(r, g, b); await this.sleep(t); this.setALL(0, 0, 0); await this.sleep(t); }
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async knightRider(loops, topRow, col) {
      for (let i = 0; i < loops; ++i) {
        await this.wave(topRow ? 'LeftToRightPunchOnly' : 'LeftToRightKickOnly', 'Medium', 0, col);
        await this.wave(topRow ? 'RightToLeftPunchOnly' : 'RightToLeftKickOnly', 'Medium', 0, col);
      }
    }
    async randomise(count, lit, gap, [r, g, b]) {
      this.setALL(0, 0, 0); await this.sleep(MIN_LED_UPDATE_DELAY);
      lit = Math.max(lit, MIN_LED_UPDATE_DELAY); gap = Math.max(gap, MIN_LED_UPDATE_DELAY);
      const L = this.cfg.leds;
      const candidates = this.cfg.ledList.filter(n => !['Up', 'Down', 'Left', 'Right'].includes(n) && L[n] !== NONE);
      if (!candidates.length) return;
      for (let i = 0; i < count; ++i) {
        const n = candidates[Math.floor(Math.random() * candidates.length)];
        this.setLED(L[n], r, g, b);
        this.updateALL();
        await this.sleep(lit);
        this.setALL(0, 0, 0);
        await this.sleep(gap);
      }
      await this.sleep(MIN_LED_UPDATE_DELAY);
    }
    async playStep(a) {
      const col = a.colour ? this.rgb(a.colour) : [0, 0, 0];
      switch (a.type) {
        case 'Wave': return this.wave(a.direction, a.speed, a.loops, col);
        case 'FlashColour': return this.flashColour(col, a.timeMs);
        case 'CircleRGB': return this.circleRGB(a.loops);
        case 'CircleOneColour': return this.circleOneColour(a.loops, col);
        case 'FlashAllSpeedIncreasing': return this.flashAllSpeedIncreasing(col);
        case 'KnightRider': return this.knightRider(a.loops, a.topRow, col);
        case 'Randomise': return this.randomise(a.count, a.litMs, a.gapMs, col);
        case 'Pause': return this.sleep(a.timeMs);
      }
    }
    async playAnimations(steps) {
      for (const a of steps) {
        for (let r = 0; r < (a.repeat || 1); ++r) await this.playStep(a);
      }
    }

    // ---- kaimana-zero.ino ----
    checkButton(name) {
      const held = this.held.has(name);
      const led = this.cfg.leds[name];
      if (led === NONE || led === undefined) return held;
      if (held) {
        if (!this.iLED[led]) {
          const c = this.colourFor('pressed', name);
          this.setLED(led, c[0], c[1], c[2]);
          this.iLED[led] = true;
        }
      } else {
        const c = this.colourFor('notPressed', name);
        const ch = this.character || {};
        this.setLED(led, c[0], c[1], c[2], false, ch.holdMs || 0, ch.fadeMs || 0);
        this.iLED[led] = false;
      }
      return held;
    }
    testCharacterCombos() {
      const moves = (this.character && this.character.moves) || [];
      for (const m of moves) {
        for (const t of m.inputs) {
          const motion = this.motions[t.motion];
          if (!motion) continue;
          if (this.switchHistoryTest(motion, t.triggers, !!t.charge)) return m;
        }
      }
      return null;
    }
    async pollSwitches() {
      this.switchHistoryBeginFrame();
      this.checkButton('Select');
      this.checkButton('Start');
      for (const b of ATTACKS) if (this.checkButton(b)) this.switchHistorySet(b);
      const up = this.held.has('Up'), down = this.held.has('Down'), left = this.held.has('Left'), right = this.held.has('Right');
      let joy = null;
      if (up && left) joy = 'UpLeft'; else if (down && left) joy = 'DownLeft'; else if (down && right) joy = 'DownRight';
      else if (up && right) joy = 'UpRight'; else if (up) joy = 'Up'; else if (down) joy = 'Down'; else if (left) joy = 'Left'; else if (right) joy = 'Right';
      if (joy) this.switchHistorySet(joy);
      ['Up', 'Left', 'Right', 'Down'].forEach(d => this.checkButton(d));
      this.switchHistoryEndFrame();

      let fired = false;
      let move = this.pendingAnimation ? null : this.testCharacterCombos();
      let steps = move ? move.animations : null;
      if (this.pendingAnimation) { steps = this.pendingAnimation; this.pendingAnimation = null; }
      if (steps) {
        fired = true;
        if (move && this.onMove) this.onMove(move);
        this.inAnimation = true;
        this.aborting = false;
        try { await this.playAnimations(steps); } catch (e) { if (!(e instanceof AbortAnimation)) throw e; this.setALL(0, 0, 0); }
        this.inAnimation = false;
        this.aborting = false;
        this.iLED.fill(false);
        for (const name of this.cfg.ledList) {
          const c = this.colourFor('notPressed', name);
          this.setLED(this.cfg.leds[name], c[0], c[1], c[2]);
        }
      }
      this.blendLEDs();
      return this.iLED.some(Boolean) || fired;
    }
    async frame() {
      if (await this.pollSwitches()) {
        if (this.millis() > this.idleTimeout) this.blendLEDs(true);
        this.idleTimeout = this.millis() + this.cfg.idleTimeoutMs;
      }
      if (this.millis() > this.idleTimeout) this.animationIdle();
      this.updateALL();
      await this.sleep(MIN_LED_UPDATE_DELAY + this.updateMs() + 0.3);
    }
    async run() {
      if (this.running) return;
      this.running = true;
      this.vnow = this.realNow();
      try {
        while (this.running) await this.frame();
      } catch (e) {
        if (!(e instanceof AbortAnimation)) throw e;
      }
    }
    stop() { this.running = false; }

    // Plays a list of animation steps as if a special move had just fired
    play(steps) {
      if (this.inAnimation) this.aborting = true;
      this.pendingAnimation = steps;
    }
    cancelAnimation() { if (this.inAnimation) this.aborting = true; }
    press(name) { this.held.add(name); }
    release(name) { this.held.delete(name); }
    releaseAll() { this.held.clear(); }
  }

  // Draws a stick with every LED of every button, using the LED layout from the config
  class StickRenderer {
    constructor(canvas, sim) {
      this.canvas = canvas;
      this.sim = sim;
      this.ctx = canvas.getContext('2d');
      this.layout = {
        Up: { x: 110, y: 150, dir: -Math.PI / 2 }, Right: { x: 110, y: 150, dir: 0 }, Down: { x: 110, y: 150, dir: Math.PI / 2 }, Left: { x: 110, y: 150, dir: Math.PI },
        P1: { x: 250, y: 128 }, P2: { x: 318, y: 106 }, P3: { x: 390, y: 106 }, P4: { x: 462, y: 116 },
        K1: { x: 240, y: 198 }, K2: { x: 308, y: 176 }, K3: { x: 380, y: 176 }, K4: { x: 452, y: 186 },
        Select: { x: 60, y: 36 }, Start: { x: 100, y: 36 }, Home: { x: 140, y: 36 },
      };
      this.hit = [];
      this.labels = true;
    }
    draw(leds) {
      const ctx = this.ctx, cfg = this.sim.cfg;
      const dpr = (typeof window !== 'undefined' && window.devicePixelRatio) || 1;
      const W = 540, H = 250;
      if (this.canvas.width !== W * dpr) { this.canvas.width = W * dpr; this.canvas.height = H * dpr; }
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      ctx.clearRect(0, 0, W, H);
      ctx.fillStyle = '#16161b';
      ctx.beginPath(); ctx.roundRect ? ctx.roundRect(4, 4, W - 8, H - 8, 18) : ctx.rect(4, 4, W - 8, H - 8); ctx.fill();
      const col = i => (leds && leds[i]) ? `rgb(${leds[i][0]},${leds[i][1]},${leds[i][2]})` : '#000';
      this.hit = [];
      // joystick directions: each direction is a quarter of a ring with its LEDs spread along it
      const jx = 110, jy = 150;
      ctx.fillStyle = '#0b0b0e'; ctx.beginPath(); ctx.arc(jx, jy, 66, 0, Math.PI * 2); ctx.fill();
      for (const d of ['Up', 'Right', 'Down', 'Left']) {
        const base = cfg.leds[d];
        if (base === NONE) continue;
        const n = cfg.perDirection;
        const span = Math.PI / 2 * 0.92;
        const a0 = this.layout[d].dir - span / 2;
        for (let k = 0; k < n; ++k) {
          ctx.strokeStyle = col(base + k);
          ctx.lineWidth = 14;
          ctx.beginPath();
          ctx.arc(jx, jy, 56, a0 + span * k / n + 0.02, a0 + span * (k + 1) / n - 0.02);
          ctx.stroke();
        }
        this.hit.push({ name: d, x: jx + Math.cos(this.layout[d].dir) * 50, y: jy + Math.sin(this.layout[d].dir) * 50, r: 22 });
      }
      ctx.fillStyle = '#c33'; ctx.beginPath(); ctx.arc(jx, jy, 20, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#e66'; ctx.beginPath(); ctx.arc(jx - 5, jy - 5, 7, 0, Math.PI * 2); ctx.fill();
      // buttons: ring of LEDs plus a fill of the first LED's colour
      for (const b of ['P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4', 'Select', 'Start', 'Home']) {
        const { x, y } = this.layout[b];
        const small = b === 'Select' || b === 'Start' || b === 'Home';
        const r = small ? 12 : 27;
        const base = cfg.leds[b];
        if (small && base === NONE) continue;
        ctx.fillStyle = '#2a2a31'; ctx.beginPath(); ctx.arc(x, y, r + 3, 0, Math.PI * 2); ctx.fill();
        if (base === NONE) {
          ctx.fillStyle = '#44444c'; ctx.beginPath(); ctx.arc(x, y, r, 0, Math.PI * 2); ctx.fill();
        } else {
          const n = cfg.perButton;
          for (let k = 0; k < n; ++k) {
            ctx.fillStyle = col(base + k);
            ctx.beginPath(); ctx.moveTo(x, y);
            ctx.arc(x, y, r, -Math.PI / 2 + Math.PI * 2 * k / n, -Math.PI / 2 + Math.PI * 2 * (k + 1) / n);
            ctx.closePath(); ctx.fill();
          }
        }
        if (this.sim.held.has(b)) { ctx.strokeStyle = '#fff'; ctx.lineWidth = 2; ctx.beginPath(); ctx.arc(x, y, r + 3, 0, Math.PI * 2); ctx.stroke(); }
        if (this.labels && !small) {
          ctx.font = '600 11px system-ui, sans-serif'; ctx.textAlign = 'center'; ctx.textBaseline = 'middle';
          ctx.fillStyle = 'rgba(0,0,0,0.55)'; ctx.fillText(b, x + 0.5, y + 0.5);
          ctx.fillStyle = 'rgba(255,255,255,0.8)'; ctx.fillText(b, x, y);
        }
        this.hit.push({ name: b, x, y, r: r + 3 });
      }
    }
    hitTest(px, py) {
      for (const h of this.hit) if ((px - h.x) ** 2 + (py - h.y) ** 2 <= h.r * h.r) return h.name;
      return null;
    }
  }

  const api = { KaimanaSim, StickRenderer, parseConfig, DEFAULT_CONFIG, COLOR_CYCLE };
  if (typeof module !== 'undefined' && module.exports) module.exports = api;
  else root.KaimanaSimulator = api;
})(typeof window !== 'undefined' ? window : globalThis);
