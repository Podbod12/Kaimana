// Kaimana character code generator.
//
// Turns the character files in characters/ into GeneratedCharacters.cpp (the PROGMEM tables read by CharacterData.cpp).
// Used by character-editor.html in the browser, and can also be run from the command line:
//
//   node editor/codegen.js            (run from the sketch folder, or pass the sketch folder as an argument)
//
// If you change a structure in CharacterData.h, change the matching emit code here.

(function (root) {
  'use strict';

  const INPUTS = ['Up', 'UpLeft', 'Left', 'DownLeft', 'Down', 'DownRight', 'Right', 'UpRight',
    'P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4'];
  const DIRECTIONS = INPUTS.slice(0, 8);
  const ATTACKS = INPUTS.slice(8);

  // Order must match GEN_SLOT_LEDS in CharacterData.cpp
  const COLOUR_SLOTS = ['P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4', 'Up', 'Down', 'Left', 'Right', 'Select', 'Start', 'Home'];

  // Order must match EIdleType in kaimana.h
  const IDLE_TYPES = ['RainbowCircling', 'RainbowPulsing', 'StaticColour', 'StaticColourPulsing',
    'StaticColourCirclePulse', 'StaticColourCircleDualPulse', 'StaticColourPingPongPulse', 'Disabled'];

  // Order must match EWaveType / EWaveSpeed in animations.h
  const WAVE_DIRECTIONS = ['LeftToRight', 'LeftToRightPunchOnly', 'LeftToRightKickOnly',
    'RightToLeft', 'RightToLeftPunchOnly', 'RightToLeftKickOnly', 'DownToUp', 'UpToDown'];
  const WAVE_SPEEDS = ['Slow', 'Medium', 'Fast'];

  // Named colours from kaimana.h, so generated comments and the editor can use friendly names
  const NAMED_COLOURS = {
    BLACK: '#000000', RED: '#ff0000', GREEN: '#00ff00', YELLOW: '#ffff00', BLUE: '#0000ff', PURPLE: '#ff00ff',
    CYAN: '#00ffff', WHITE: '#ffffff', ORANGE: '#dc7f00', GOLD: '#ff9600', BROWN: '#f0e68c', GREY: '#7f7f7f',
    DARKGREY: '#3c3c3c', DARKBLUE: '#000064',
  };

  // Matches randomColors in kaimana_custom.h. Used by the preview for the "random" colour
  const RANDOM_COLOURS = ['#7fdc00', '#7f00dc', '#00dcdc', '#00ff7f', '#007fff', '#ff0000', '#dc7f00', '#dc007f', '#b1254b'];

  const COLOUR_FIELD = { key: 'colour', label: 'Colour', kind: 'colour', default: '#ffffff' };

  // Animation step types. id must match EGenAnimType in CharacterData.h.
  // encode() packs the step into the GenAnim p0/p1/p2 fields.
  const ANIMATIONS = {
    Wave: {
      id: 0, label: 'Wave (fireball)',
      help: 'A wave of colour across the buttons. Loops > 0 strobes it first (used for supers).',
      fields: [
        { key: 'direction', label: 'Direction', kind: 'select', options: WAVE_DIRECTIONS, default: 'LeftToRight' },
        { key: 'speed', label: 'Speed', kind: 'select', options: WAVE_SPEEDS, default: 'Medium' },
        { key: 'loops', label: 'Strobe loops', kind: 'int', min: 0, max: 255, default: 0 },
        COLOUR_FIELD],
      encode: s => ({ p0: WAVE_DIRECTIONS.indexOf(s.direction) | (WAVE_SPEEDS.indexOf(s.speed) << 4), p1: s.loops }),
    },
    FlashColour: {
      id: 1, label: 'Flash all', help: 'Light every button one colour for a time.',
      fields: [COLOUR_FIELD, { key: 'timeMs', label: 'Time (ms)', kind: 'int', min: 5, max: 10000, default: 250 }],
      encode: s => ({ p1: s.timeMs }),
    },
    CircleRGB: {
      id: 2, label: 'Circle RGB', help: 'Circle the attack buttons in red, green then blue. Plays loops + 1 times.',
      fields: [{ key: 'loops', label: 'Extra loops', kind: 'int', min: 0, max: 255, default: 0 }],
      encode: s => ({ p1: s.loops }),
    },
    CircleOneColour: {
      id: 3, label: 'Circle', help: 'Circle the attack buttons in one colour. Plays loops + 1 times.',
      fields: [{ key: 'loops', label: 'Extra loops', kind: 'int', min: 0, max: 255, default: 1 }, COLOUR_FIELD],
      encode: s => ({ p1: s.loops }),
    },
    FlashAllSpeedIncreasing: {
      id: 4, label: 'Flash faster and faster', help: 'Flash every button, getting quicker each time.',
      fields: [COLOUR_FIELD],
      encode: () => ({}),
    },
    KnightRider: {
      id: 5, label: 'Knight Rider', help: 'Sweep along one row and back.',
      fields: [
        { key: 'loops', label: 'Loops', kind: 'int', min: 1, max: 255, default: 1 },
        { key: 'topRow', label: 'Top row (punches)', kind: 'bool', default: true },
        COLOUR_FIELD],
      encode: s => ({ p0: s.topRow ? 1 : 0, p1: s.loops }),
    },
    Randomise: {
      id: 6, label: 'Random flashes', help: 'Flash random attack buttons one at a time.',
      fields: [
        { key: 'count', label: 'Flashes', kind: 'int', min: 1, max: 255, default: 12 },
        { key: 'litMs', label: 'Lit (ms)', kind: 'int', min: 5, max: 10000, default: 70 },
        { key: 'gapMs', label: 'Gap (ms)', kind: 'int', min: 5, max: 10000, default: 45 },
        COLOUR_FIELD],
      encode: s => ({ p0: s.count, p1: s.litMs, p2: s.gapMs }),
    },
    Pause: {
      id: 7, label: 'Pause', help: 'Wait before the next step.',
      fields: [{ key: 'timeMs', label: 'Time (ms)', kind: 'int', min: 0, max: 10000, default: 100 }],
      encode: s => ({ p1: s.timeMs }),
    },
  };

  const LIMITS = {
    slots: 8,
    maxRepeat: 16,
    maxTriggers: 4,         // GEN_MAX_TRIGGERS
    switchHistoryMax: 24,   // SWITCH_HISTORY_MAX in kaimana.h
    maxMotions: 127,        // top bit of GenTest::motion is the charge flag
    maxPalette: 255,        // 0xFF is GEN_COLOUR_RANDOM
    maxTableIndex: 255,
  };

  // Byte sizes of each table entry (packed structs in CharacterData.h)
  const SIZES = { palette: 3, colourSet: COLOUR_SLOTS.length, motionData: 1, motion: 3, triggerSet: 1 + LIMITS.maxTriggers, test: 2, anim: 7, move: 6, character: 10 };

  // Flash used by the firmware with the character tables empty, measured with arduino-cli for arduino:avr:leonardo 1.8.8.
  // Only used for the editor's estimate, the Arduino IDE's number after compiling is the real one.
  const FIRMWARE_BASE_BYTES = 17175;
  const BYTES_PER_PROFILE = 16;          // constructing each DataCharacter
  const BYTES_PER_NATIVE_PROFILE = 1500; // rough size of a hand written C++ character like Ryu.cpp
  const FLASH_LIMIT_BYTES = 28672;

  const ID_RE = /^[a-z][a-z0-9_]{0,30}$/;

  // Folder names follow the Arduino sketch naming rules so a folder can always be used as a real folder under src/:
  // start with a letter, digit or _, then letters, digits, _ . or -, at most 63 characters, not ending in a dot
  // and not a reserved Windows name.
  const FOLDER_RE = /^[A-Za-z0-9_][A-Za-z0-9_.-]{0,62}$/;
  const RESERVED_NAMES = /^(con|prn|aux|nul|com[1-9]|lpt[1-9])(\..*)?$/i;
  function folderPathError(path) {
    for (const part of path.split('/')) {
      const e = folderNameError(part);
      if (e) return e;
    }
    return null;
  }
  function folderNameError(name) {
    if (!FOLDER_RE.test(name)) return 'must start with a letter, number or _ and only use letters, numbers, _ . and - (max 63 characters)';
    if (name.endsWith('.')) return 'cannot end with a dot';
    if (RESERVED_NAMES.test(name)) return 'is a reserved Windows name';
    return null;
  }
  const HEX_RE = /^#[0-9a-fA-F]{6}$/;

  function hexToRgb(hex) {
    return [1, 3, 5].map(i => parseInt(hex.substr(i, 2), 16));
  }

  function colourName(hex) {
    const lower = hex.toLowerCase();
    for (const name in NAMED_COLOURS) {
      if (NAMED_COLOURS[name] === lower) return name;
    }
    return lower;
  }

  function resolveColour(map, slot, fallback) {
    if (!map) return fallback;
    if (map[slot] !== undefined) return map[slot];
    if (map.default !== undefined) return map.default;
    return fallback;
  }

  function isColour(value, allowRandom) {
    return (allowRandom && value === 'random') || (typeof value === 'string' && HEX_RE.test(value));
  }

  function safeComment(text) {
    return String(text || '').replace(/[\r\n\\]+/g, ' ').trim();
  }

  // A character is identified by its folder and id together (the path of its file), so two characters with the same id in
  // different folders (eg StreetFighter6/ryu and Dave/ryu) are different characters. Slots store this key.
  function characterKey(c) {
    return (c.folder ? c.folder + '/' : '') + c.id;
  }

  function parseSlot(slot) {
    if (typeof slot === 'string' && slot.startsWith('native:')) return { native: slot.slice(7) };
    return { id: slot };
  }

  // Validates a document (all the character files combined, see assembleProject). Returns a list of error strings (empty when valid).
  // options.natives maps each hand written C++ class to the header that declares it (null when it is in Characters.h).
  // Leave it out when the headers could not be read, then slots using C++ classes are reported as errors.
  function validate(doc, options) {
    const errors = [];
    const natives = options && options.natives;
    if (!doc || typeof doc !== 'object') return ['Document is not an object'];
    const motions = doc.motions || {};
    const characters = doc.characters || [];

    for (const name in motions) {
      if (!/^[\w -]{1,40}$/.test(name)) errors.push(`Motion "${name}" has an invalid name (letters, digits, spaces, - and _ only)`);
      if (!Array.isArray(motions[name])) { errors.push(`Motion "${name}" is not a list of inputs`); continue; }
      motions[name].forEach(i => { if (!INPUTS.includes(i)) errors.push(`Motion "${name}" has unknown input "${i}"`); });
    }

    const keys = new Set();
    characters.forEach((c, ci) => {
      const where = `Character "${c.name || c.id || ci}"`;
      if (!ID_RE.test(c.id || '')) errors.push(`${where} has an invalid id (lower case letters, digits and _ only, starting with a letter)`);
      else if (!c.folder && RESERVED_IDS.includes(c.id)) errors.push(`${where} cannot use the id "${c.id}" outside a folder as that file name is reserved`);
      if (c.folder !== undefined && c.folder !== '') {
        const folderError = typeof c.folder === 'string' ? folderPathError(c.folder) : 'must be text';
        if (folderError) errors.push(`${where} folder "${c.folder}" ${folderError}`);
      }
      if (keys.has(characterKey(c))) errors.push(`${where} id "${c.id}" is used more than once in the same folder`);
      keys.add(characterKey(c));
      const idle = c.idle || {};
      if (!IDLE_TYPES.includes(idle.type)) errors.push(`${where} has unknown idle type "${idle.type}"`);
      const maps = [['idle static colour', idle.staticColour, false], ['idle pulse colour', idle.pulseColour, false],
        ['not pressed colour', c.notPressedColour, false], ['pressed colour', c.pressedColour, true]];
      for (const [label, map, allowRandom] of maps) {
        for (const key in (map || {})) {
          if (key !== 'default' && !COLOUR_SLOTS.includes(key)) errors.push(`${where} ${label} has unknown button "${key}"`);
          if (!isColour(map[key], allowRandom)) errors.push(`${where} ${label} "${key}" is not a colour`);
        }
      }
      for (const key of ['holdMs', 'fadeMs']) {
        const v = c[key] || 0;
        if (!Number.isInteger(v) || v < 0 || v > 65535) errors.push(`${where} ${key} must be a whole number from 0 to 65535`);
      }
      (c.moves || []).forEach((m, mi) => {
        const mwhere = `${where} move "${m.name || mi + 1}"`;
        if (!m.inputs || !m.inputs.length) errors.push(`${mwhere} has no inputs`);
        if (!m.animations || !m.animations.length) errors.push(`${mwhere} has no animation`);
        if ((m.inputs || []).length > LIMITS.maxTableIndex) errors.push(`${mwhere} has too many inputs`);
        if ((m.animations || []).length > LIMITS.maxTableIndex) errors.push(`${mwhere} has too many animation steps`);
        (m.inputs || []).forEach((t, ti) => {
          const twhere = `${mwhere} input ${ti + 1}`;
          if (!motions[t.motion]) errors.push(`${twhere} uses unknown motion "${t.motion}"`);
          const triggers = t.triggers || [];
          if (triggers.length > LIMITS.maxTriggers) errors.push(`${twhere} has more than ${LIMITS.maxTriggers} buttons`);
          triggers.forEach(b => { if (!INPUTS.includes(b)) errors.push(`${twhere} has unknown button "${b}"`); });
          if (motions[t.motion] && motions[t.motion].length + triggers.length > LIMITS.switchHistoryMax) errors.push(`${twhere} is longer than ${LIMITS.switchHistoryMax} inputs`);
          if (motions[t.motion] && motions[t.motion].length + triggers.length === 0) errors.push(`${twhere} needs a motion or at least one button`);
          if (t.charge && motions[t.motion] && !DIRECTIONS.includes(motions[t.motion][0])) errors.push(`${twhere} is a charge move so its motion must start with a direction`);
        });
        (m.animations || []).forEach((a, ai) => {
          const awhere = `${mwhere} animation ${ai + 1}`;
          const def = ANIMATIONS[a.type];
          if (!def) { errors.push(`${awhere} has unknown type "${a.type}"`); return; }
          for (const f of def.fields) {
            const v = a[f.key];
            if (f.kind === 'colour' && !isColour(v, true)) errors.push(`${awhere} ${f.label} is not a colour`);
            if (f.kind === 'int' && (!Number.isInteger(v) || v < f.min || v > f.max)) errors.push(`${awhere} ${f.label} must be a whole number from ${f.min} to ${f.max}`);
            if (f.kind === 'select' && !f.options.includes(v)) errors.push(`${awhere} ${f.label} "${v}" is not valid`);
            if (f.kind === 'bool' && typeof v !== 'boolean') errors.push(`${awhere} ${f.label} must be true or false`);
          }
          const r = a.repeat === undefined ? 1 : a.repeat;
          if (!Number.isInteger(r) || r < 1 || r > LIMITS.maxRepeat) errors.push(`${awhere} repeat must be from 1 to ${LIMITS.maxRepeat}`);
        });
      });
    });

    const slots = doc.slots || [];
    if (slots.length !== LIMITS.slots) errors.push(`There must be exactly ${LIMITS.slots} profile slots (found ${slots.length})`);
    slots.forEach((s, i) => {
      const slot = parseSlot(s);
      if (slot.native !== undefined) {
        if (!/^[A-Za-z_]\w*$/.test(slot.native)) errors.push(`Slot ${i + 1} has an invalid C++ class name "${slot.native}"`);
        else if (!natives) errors.push(`Slot ${i + 1} uses C++ class "${slot.native}". Open the sketch folder so the editor can find its header in src/`);
        else if (!Object.prototype.hasOwnProperty.call(natives, slot.native)) errors.push(`Slot ${i + 1} uses C++ class "${slot.native}" which was not found in src/ or Characters.h`);
      } else if (!keys.has(slot.id)) {
        errors.push(`Slot ${i + 1} uses unknown character "${slot.id}"`);
      }
    });
    return errors;
  }

  // Adds items to a table, reusing an existing entry when the key matches
  class Table {
    constructor() { this.items = []; this.index = new Map(); }
    add(key, item) {
      if (this.index.has(key)) return this.index.get(key);
      this.items.push(item);
      this.index.set(key, this.items.length - 1);
      return this.items.length - 1;
    }
  }

  // Adds a run of entries to a flat table, reusing an identical earlier run
  class RunTable {
    constructor() { this.items = []; this.runs = new Map(); }
    add(keys, items) {
      const key = keys.join('|');
      if (this.runs.has(key)) return this.runs.get(key);
      const first = this.items.length;
      this.items.push(...items);
      this.runs.set(key, first);
      return first;
    }
  }

  // Builds all the tables for the characters used in the slots
  function build(doc, natives) {
    const motions = doc.motions;
    const byKey = new Map(doc.characters.map(c => [characterKey(c), c]));
    // C++ variable name for each character, unique even if two keys only differ by punctuation
    const varNames = new Map();
    const varName = key => {
      if (!varNames.has(key)) {
        const base = 'gen_' + key.replace(/[^A-Za-z0-9_]/g, '_');
        let n = base, i = 2;
        while ([...varNames.values()].includes(n)) n = `${base}_${i++}`;
        varNames.set(key, n);
      }
      return varNames.get(key);
    };

    const palette = new Table();
    palette.add('#000000', '#000000'); // always have entry 0 so steps without a colour have something to point at
    const colourSets = new Table();
    const motionTable = new Table();
    const motionData = [];
    const triggerSets = new Table();
    const tests = new RunTable();
    const anims = new RunTable();
    const moves = [];
    const genCharacters = [];
    const instances = []; // { name, ctor, comment }
    const slotInstances = [];

    const paletteIndex = hex => (hex === 'random' ? 0xFF : palette.add(hex.toLowerCase(), hex.toLowerCase()));

    const motionIndex = name => {
      if (motionTable.index.has(name)) return motionTable.index.get(name);
      const seq = motions[name];
      const entry = { name, offset: motionData.length, length: seq.length };
      motionData.push(...seq);
      return motionTable.add(name, entry);
    };

    const colourSetIndex = (map, fallback) => {
      const indices = COLOUR_SLOTS.map(s => paletteIndex(resolveColour(map, s, fallback)));
      return colourSets.add(indices.join(','), indices);
    };

    for (const s of doc.slots) {
      const slot = parseSlot(s);
      if (slot.native !== undefined) {
        const name = 'native_' + slot.native;
        const header = natives ? natives[slot.native] : null;
        if (!instances.some(i => i.name === name)) instances.push({ name, type: slot.native, header, comment: `Hand written C++ character from ${header || 'Characters.h'}` });
        slotInstances.push(name);
        continue;
      }
      const name = varName(slot.id);
      if (instances.some(i => i.name === name)) { slotInstances.push(name); continue; }

      const c = byKey.get(slot.id);
      const idle = c.idle || {};
      const gc = {
        name: c.name || c.id,
        idleType: IDLE_TYPES.indexOf(idle.type),
        colourSets: [
          colourSetIndex(idle.staticColour, '#000000'),
          colourSetIndex(idle.pulseColour, '#ffffff'),
          colourSetIndex(c.notPressedColour, '#000000'),
          colourSetIndex(c.pressedColour, 'random'),
        ],
        holdMs: c.holdMs || 0,
        fadeMs: c.fadeMs || 0,
        firstMove: moves.length,
        moveCount: (c.moves || []).length,
      };
      for (const m of (c.moves || [])) {
        const testItems = m.inputs.map(t => ({
          motion: motionIndex(t.motion) | (t.charge ? 0x80 : 0),
          triggerSet: triggerSets.add(t.triggers.join(','), t.triggers.slice()),
          label: `${t.motion}${t.charge ? ' (charge)' : ''} + ${t.triggers.join('+') || 'nothing'}`,
        }));
        const animItems = m.animations.map(a => {
          const def = ANIMATIONS[a.type];
          const packed = Object.assign({ p0: 0, p1: 0, p2: 0 }, def.encode(a));
          const hasColour = def.fields.some(f => f.kind === 'colour');
          return {
            type: a.type,
            typeAndRepeat: def.id | (((a.repeat || 1) - 1) << 4),
            p0: packed.p0, p1: packed.p1, p2: packed.p2,
            colour: hasColour ? paletteIndex(a.colour) : 0,
          };
        });
        moves.push({
          name: m.name,
          firstTest: tests.add(testItems.map(t => `${t.motion},${t.triggerSet}`), testItems),
          testCount: testItems.length,
          firstAnim: anims.add(animItems.map(a => `${a.typeAndRepeat},${a.p0},${a.p1},${a.p2},${a.colour}`), animItems),
          animCount: animItems.length,
        });
      }
      instances.push({ name, index: genCharacters.length, comment: gc.name });
      genCharacters.push(gc);
      slotInstances.push(name);
    }

    return {
      palette: palette.items, colourSets: colourSets.items, motions: motionTable.items, motionData,
      triggerSets: triggerSets.items, tests: tests.items, anims: anims.items, moves, characters: genCharacters,
      instances, slotInstances,
    };
  }

  function tableLimitErrors(t) {
    const errors = [];
    if (t.palette.length > LIMITS.maxPalette) errors.push(`Too many different colours (${t.palette.length}, max ${LIMITS.maxPalette})`);
    if (t.motions.length > LIMITS.maxMotions) errors.push(`Too many different motions (${t.motions.length}, max ${LIMITS.maxMotions})`);
    if (t.triggerSets.length > LIMITS.maxTableIndex) errors.push(`Too many different button combinations (${t.triggerSets.length})`);
    if (t.colourSets.length > LIMITS.maxTableIndex) errors.push(`Too many different colour layouts (${t.colourSets.length})`);
    t.characters.forEach(c => { if (c.moveCount > LIMITS.maxTableIndex) errors.push(`${c.name} has more than ${LIMITS.maxTableIndex} moves`); });
    return errors;
  }

  function tableBytes(t) {
    return {
      palette: t.palette.length * SIZES.palette,
      colourSets: t.colourSets.length * SIZES.colourSet,
      motions: t.motionData.length * SIZES.motionData + t.motions.length * SIZES.motion,
      triggerSets: t.triggerSets.length * SIZES.triggerSet,
      tests: t.tests.length * SIZES.test,
      anims: t.anims.length * SIZES.anim,
      moves: t.moves.length * SIZES.move,
      characters: t.characters.length * SIZES.character,
    };
  }

  // Works out the flash/RAM cost of the current slots without generating code
  function estimate(doc) {
    const t = build(doc);
    const bytes = tableBytes(t);
    const total = Object.values(bytes).reduce((a, b) => a + b, 0);
    const natives = t.instances.filter(i => i.type).length;
    const profiles = t.instances.length - natives;
    return {
      tableBytes: total,
      breakdown: bytes,
      estimatedFlash: FIRMWARE_BASE_BYTES + total + profiles * BYTES_PER_PROFILE + natives * BYTES_PER_NATIVE_PROFILE,
      flashLimit: FLASH_LIMIT_BYTES,
      uniqueProfiles: t.instances.length,
      nativeProfiles: natives,
      counts: { moves: t.moves.length, tests: t.tests.length, anims: t.anims.length, colours: t.palette.length },
    };
  }

  function inputEnum(i) { return 'EIT_Input_' + i; }

  function emit(t, doc) {
    const L = [];
    const hr = '//' + '/'.repeat(118);
    L.push('//  GeneratedCharacters.cpp');
    L.push('//');
    L.push('//  AUTO GENERATED FILE - DO NOT EDIT BY HAND.');
    L.push('//  Written by editor/character-editor.html (or node editor/codegen.js) from the files in characters/.');
    L.push('//  Change your characters in the editor and save again, any edits made here will be lost.');
    L.push('//');
    L.push('//  Profile slots (hold P1+P4+K1+K4 then press the button to pick one):');
    ['P1', 'P2', 'P3', 'P4', 'K1', 'K2', 'K3', 'K4'].forEach((b, i) => {
      const inst = t.instances.find(x => x.name === t.slotInstances[i]);
      L.push(`//    ${b} : ${safeComment(inst.comment)}${inst.type ? ` (${inst.type})` : ''}`);
    });
    L.push('//');
    L.push('');
    L.push('#define __PROG_TYPES_COMPAT__');
    L.push('#include <avr/pgmspace.h>');
    L.push('#include "CharacterData.h"');
    const headers = [...new Set(t.instances.map(i => i.header).filter(Boolean))];
    if (headers.length) {
      L.push('');
      L.push('// Hand written C++ characters used in the slots');
      headers.forEach(hp => L.push(`#include "${hp}"`));
    }
    L.push('');

    L.push(hr);
    L.push('// Colours');
    L.push('const RGB_t GEN_PALETTE[] PROGMEM = {');
    t.palette.forEach((hex, i) => {
      const [r, g, b] = hexToRgb(hex);
      L.push(`  { ${String(r).padStart(3)}, ${String(g).padStart(3)}, ${String(b).padStart(3)} }, // ${i} ${colourName(hex)}`);
    });
    L.push('};');
    L.push('');
    L.push('// Colour of each button : P1, P2, P3, P4, K1, K2, K3, K4, Up, Down, Left, Right, Select, Start, Home (255 = random)');
    L.push('const uint8_t GEN_COLOUR_SETS[][GEN_COLOUR_SLOTS] PROGMEM = {');
    (t.colourSets.length ? t.colourSets : [COLOUR_SLOTS.map(() => 0)]).forEach((set, i) => {
      L.push(`  { ${set.map(v => String(v).padStart(3)).join(', ')} }, // ${i}`);
    });
    L.push('};');
    L.push('');

    L.push(hr);
    L.push('// Motions (the joystick part of a special move)');
    L.push('const EInputTypes GEN_MOTION_DATA[] PROGMEM = {');
    if (!t.motionData.length) L.push('  EIT_INPUT_NONE,');
    t.motions.forEach(m => {
      const seq = t.motionData.slice(m.offset, m.offset + m.length);
      if (seq.length) L.push(`  ${seq.map(inputEnum).join(', ')}, // ${m.name}`);
    });
    L.push('};');
    L.push('');
    L.push('const GenMotion GEN_MOTIONS[] PROGMEM = {');
    if (!t.motions.length) L.push('  { 0, 0 },');
    t.motions.forEach((m, i) => L.push(`  { ${m.offset}, ${m.length} }, // ${i} ${m.name}`));
    L.push('};');
    L.push('');
    L.push('// Buttons that finish a move');
    L.push('const GenTriggerSet GEN_TRIGGER_SETS[] PROGMEM = {');
    if (!t.triggerSets.length) L.push('  { 0, { EIT_INPUT_NONE } },');
    t.triggerSets.forEach((set, i) => {
      L.push(`  { ${set.length}, { ${set.length ? set.map(inputEnum).join(', ') : 'EIT_INPUT_NONE'} } }, // ${i}`);
    });
    L.push('};');
    L.push('');

    L.push(hr);
    L.push('// Ways to perform each move : { motion index (+128 if charge), button set index }');
    L.push('const GenTest GEN_TESTS[] PROGMEM = {');
    if (!t.tests.length) L.push('  { 0, 0 },');
    t.tests.forEach((x, i) => L.push(`  { ${String(x.motion).padStart(3)}, ${String(x.triggerSet).padStart(3)} }, // ${i} ${safeComment(x.label)}`));
    L.push('};');
    L.push('');
    L.push('// Animation steps : { type | (repeat - 1) << 4, p0, p1, p2, colour } - see EGenAnimType in CharacterData.h');
    L.push('const GenAnim GEN_ANIMS[] PROGMEM = {');
    if (!t.anims.length) L.push('  { GAT_Pause, 0, 0, 0, 0 },');
    t.anims.forEach((a, i) => {
      const typeExpr = `GAT_${a.type}` + ((a.typeAndRepeat >> 4) ? ` | (${a.typeAndRepeat >> 4} << 4)` : '');
      L.push(`  { ${typeExpr}, ${a.p0}, ${a.p1}, ${a.p2}, ${a.colour} }, // ${i}`);
    });
    L.push('};');
    L.push('');
    L.push('// Special moves : { first test, test count, first animation, animation count }');
    L.push('const GenMove GEN_MOVES[] PROGMEM = {');
    if (!t.moves.length) L.push('  { 0, 0, 0, 0 },');
    let moveIndex = 0;
    t.characters.forEach(c => {
      if (c.moveCount) L.push(`  // ${safeComment(c.name)}`);
      for (let i = 0; i < c.moveCount; ++i, ++moveIndex) {
        const m = t.moves[moveIndex];
        L.push(`  { ${m.firstTest}, ${m.testCount}, ${m.firstAnim}, ${m.animCount} }, // ${moveIndex} ${safeComment(m.name)}`);
      }
    });
    L.push('};');
    L.push('');

    L.push(hr);
    L.push('// Characters : { idle type, { idle static, idle pulse, not pressed, pressed colour sets }, hold ms, fade ms, first move, move count }');
    L.push('const GenCharacter GEN_CHARACTERS[] PROGMEM = {');
    if (!t.characters.length) L.push('  { EIT_Disabled, { 0, 0, 0, 0 }, 0, 0, 0, 0 },');
    t.characters.forEach((c, i) => {
      L.push(`  { EIT_${IDLE_TYPES[c.idleType]}, { ${c.colourSets.join(', ')} }, ${c.holdMs}, ${c.fadeMs}, ${c.firstMove}, ${c.moveCount} }, // ${i} ${safeComment(c.name)}`);
    });
    L.push('};');
    L.push('');

    L.push(hr);
    L.push('// Profiles');
    t.instances.forEach(inst => {
      if (inst.type) L.push(`${inst.type} ${inst.name}; // ${safeComment(inst.comment)}`);
      else L.push(`DataCharacter ${inst.name}(&GEN_CHARACTERS[${inst.index}]); // ${safeComment(inst.comment)}`);
    });
    L.push('');
    L.push(`const Character* AllCharacters[NUM_CHARACTERS] = { ${t.slotInstances.map(n => '&' + n).join(', ')} };`);
    L.push('');
    return L.join('\n');
  }

  // Generates GeneratedCharacters.cpp. Returns { code, errors, stats }. code is null when there are errors.
  function generate(doc, options) {
    const errors = validate(doc, options);
    if (errors.length) return { code: null, errors, stats: null };
    const t = build(doc, options && options.natives);
    const limitErrors = tableLimitErrors(t);
    if (limitErrors.length) return { code: null, errors: limitErrors, stats: null };
    return { code: emit(t, doc), errors: [], stats: estimate(doc) };
  }

  // Finds the hand written character classes in Characters.h so they can be put in a slot
  function parseNativeClasses(charactersH) {
    const noComments = charactersH.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/[^\n]*/g, '');
    const out = [];
    const re = /class\s+(\w+)\s*:\s*public\s+Character\b/g;
    let m;
    while ((m = re.exec(noComments))) out.push(m[1]);
    return out;
  }

  // Builds the options.natives map from the sketch's headers.
  // files is a list of { path, text } with paths relative to the sketch folder using / (eg "src/StreetFighter6/Ryu.h").
  // Classes in Characters.h map to null as GeneratedCharacters.cpp already includes it.
  function findNativeClasses(files) {
    const natives = {};
    for (const f of files) {
      const header = f.path === 'Characters.h' ? null : f.path;
      for (const cls of parseNativeClasses(f.text)) natives[cls] = header;
    }
    return natives;
  }

  // Folder name of a C++ class for grouping in the editor, eg "StreetFighter6" for src/StreetFighter6/Ryu.h
  function nativeFolder(header) {
    if (!header) return 'Characters.h';
    const parts = header.split('/');
    return parts.length > 2 ? parts.slice(1, -1).join('/') : parts[0];
  }

  // ---- Project files ----
  //
  // Characters are stored one per file so they can be shared by dropping a file into the folder:
  //   characters/profiles.json            the 8 profile slots
  //   characters/motions.json             the shared motion library
  //   characters/<Folder>/<id>.json       one character. The folder it sits in is its folder (none if directly in characters/)
  // Each character file also carries the motions it uses, so a character sent by someone else works even if your
  // motion library doesn't have them.

  const PROJECT_DIR = 'characters';
  const PROFILES_FILE = PROJECT_DIR + '/profiles.json';
  const MOTIONS_FILE = PROJECT_DIR + '/motions.json';
  const RESERVED_IDS = ['profiles', 'motions'];
  const CHARACTER_FIELDS = ['id', 'name', 'notes', 'idle', 'notPressedColour', 'pressedColour', 'holdMs', 'fadeMs', 'moves'];

  const toJson = obj => JSON.stringify(obj, null, 2) + '\n';

  function characterPath(c) {
    return `${PROJECT_DIR}/${c.folder ? c.folder + '/' : ''}${c.id}.json`;
  }

  // Splits a document into { path: text } for every project file
  function splitProject(doc) {
    const files = {};
    files[PROFILES_FILE] = toJson({ format: 'kaimana-profiles', version: 1, slots: doc.slots });
    files[MOTIONS_FILE] = toJson({ format: 'kaimana-motions', version: 1, motions: doc.motions });
    for (const c of doc.characters) {
      const out = { format: 'kaimana-character', version: 1 };
      for (const k of CHARACTER_FIELDS) if (c[k] !== undefined) out[k] = c[k];
      const used = {};
      for (const m of c.moves || []) for (const t of m.inputs || []) if (doc.motions[t.motion]) used[t.motion] = doc.motions[t.motion];
      out.motions = used;
      files[characterPath(c)] = toJson(out);
    }
    return files;
  }

  // Builds one document from the project files.
  // sources: { profiles: text|null, motions: text|null, characters: [{ path, text }], legacy: text|null (old single characters.json) }
  // Returns { doc, warnings }. Problems that can be fixed automatically (clashing motions, slots named by id only) are fixed and reported.
  function assembleProject(sources) {
    const warnings = [];
    const parse = (text, what) => {
      try { return JSON.parse(text); } catch (e) { warnings.push(`${what} is not valid JSON (${e.message}), it was skipped`); return null; }
    };

    if (sources.legacy && !(sources.characters || []).length) {
      const doc = parse(sources.legacy, 'characters.json');
      if (!doc) return { doc: null, warnings };
      doc.characters = doc.characters || [];
      doc.motions = doc.motions || {};
      doc.slots = resolveSlots(doc.slots || [], doc.characters, warnings);
      warnings.push('Loaded the old single characters.json. Saving will split it into one file per character in characters/.');
      return { doc, warnings };
    }

    const motionsDoc = sources.motions ? parse(sources.motions, MOTIONS_FILE) : null;
    const motions = Object.assign({}, (motionsDoc && motionsDoc.motions) || {});
    const characters = [];
    const keys = new Set();
    const files = (sources.characters || []).slice().sort((a, b) => a.path.localeCompare(b.path));
    for (const f of files) {
      const c = parse(f.text, f.path);
      if (!c) continue;
      if (c.format && c.format !== 'kaimana-character') { warnings.push(`${f.path} is not a Kaimana character file, it was skipped`); continue; }
      const parts = f.path.split('/');
      const fileId = parts[parts.length - 1].replace(/\.json$/i, '');
      const folder = parts.slice(1, -1).join('/');
      const character = {};
      for (const k of CHARACTER_FIELDS) if (c[k] !== undefined) character[k] = c[k];
      character.id = character.id || fileId;
      if (folder) character.folder = folder;
      character.moves = character.moves || [];

      const key = characterKey(character);
      if (keys.has(key)) { warnings.push(`${f.path} is a second character with the id "${character.id}" in the same folder, it was skipped`); continue; }
      keys.add(key);

      // bring in the motions this character carries
      const rename = {};
      for (const name in (c.motions || {})) {
        const seq = c.motions[name];
        if (!motions[name]) { motions[name] = seq; continue; }
        if (motions[name].join(',') === seq.join(',')) continue;
        let newName = `${name}_${key.toUpperCase().replace(/[^A-Z0-9_]/g, '_')}`;
        while (motions[newName] && motions[newName].join(',') !== seq.join(',')) newName += '_';
        motions[newName] = seq;
        rename[name] = newName;
        warnings.push(`${f.path} has its own version of the motion "${name}", so it was kept as "${newName}" for ${character.name || character.id}`);
      }
      for (const m of character.moves) for (const t of m.inputs || []) if (rename[t.motion]) t.motion = rename[t.motion];
      characters.push(character);
    }

    characters.sort((a, b) => (a.folder || '').localeCompare(b.folder || '') || (a.name || a.id).localeCompare(b.name || b.id));

    const profilesDoc = sources.profiles ? parse(sources.profiles, PROFILES_FILE) : null;
    let slots = profilesDoc && profilesDoc.slots;
    if (!Array.isArray(slots)) {
      slots = [];
      for (let i = 0; i < LIMITS.slots && characters.length; ++i) slots.push(characterKey(characters[i % characters.length]));
      if (characters.length) warnings.push(`No ${PROFILES_FILE} found, so the first characters were put in the profile slots`);
    } else {
      slots = resolveSlots(slots, characters, warnings);
    }
    return { doc: { format: 'kaimana-characters', version: 1, slots, motions, characters }, warnings };
  }

  // A slot can name a character by id alone (eg "ryu", as older files did) as long as only one character has that id
  function resolveSlots(slots, characters, warnings) {
    const keys = new Set(characters.map(characterKey));
    return slots.map(slot => {
      if (typeof slot !== 'string' || slot.startsWith('native:') || keys.has(slot)) return slot;
      const matches = characters.filter(c => c.id === slot);
      if (matches.length === 1) return characterKey(matches[0]);
      if (matches.length > 1) warnings.push(`Profile slot "${slot}" could be any of ${matches.map(characterKey).join(', ')}. Pick one for that slot.`);
      return slot;
    });
  }

  // Path helpers for callers reading a sketch folder
  function isCharacterFile(path) {
    return path.startsWith(PROJECT_DIR + '/') && /\.json$/i.test(path) && path !== PROFILES_FILE && path !== MOTIONS_FILE;
  }

  // ---- Editing helpers used by the editor ----

  const MIRROR_INPUT = { UpLeft: 'UpRight', UpRight: 'UpLeft', Left: 'Right', Right: 'Left', DownLeft: 'DownRight', DownRight: 'DownLeft' };
  const MIRROR_WAVE = {
    LeftToRight: 'RightToLeft', RightToLeft: 'LeftToRight', LeftToRightPunchOnly: 'RightToLeftPunchOnly',
    RightToLeftPunchOnly: 'LeftToRightPunchOnly', LeftToRightKickOnly: 'RightToLeftKickOnly', RightToLeftKickOnly: 'LeftToRightKickOnly',
  };

  // Returns the name of a motion that is the mirror image of the named one, adding it to doc.motions if needed
  function mirrorMotion(doc, name) {
    const seq = doc.motions[name].map(i => MIRROR_INPUT[i] || i);
    const key = seq.join(',');
    for (const other in doc.motions) {
      if (doc.motions[other].join(',') === key) return other;
    }
    let newName = name.replace(/LEFT|RIGHT/g, w => (w === 'LEFT' ? 'RIGHT' : 'LEFT'));
    if (newName === name || doc.motions[newName]) newName = name + '_MIRROR';
    while (doc.motions[newName]) newName += '_';
    doc.motions[newName] = seq;
    return newName;
  }

  // Makes a copy of a move for the other side of the screen: left/right motions and wave directions are swapped
  function mirrorMove(doc, move) {
    const copy = JSON.parse(JSON.stringify(move));
    copy.inputs.forEach(t => { t.motion = mirrorMotion(doc, t.motion); });
    copy.animations.forEach(a => { if (a.type === 'Wave' && MIRROR_WAVE[a.direction]) a.direction = MIRROR_WAVE[a.direction]; });
    copy.name = /left/i.test(move.name) ? move.name.replace(/left/gi, w => (w[0] === 'L' ? 'Right' : 'right'))
      : /right/i.test(move.name) ? move.name.replace(/right/gi, w => (w[0] === 'R' ? 'Left' : 'left')) : move.name + ' (mirrored)';
    return copy;
  }

  function defaultAnimation(type) {
    const a = { type };
    ANIMATIONS[type].fields.forEach(f => { a[f.key] = f.default; });
    return a;
  }

  function newCharacter(id, name, folder) {
    return {
      id, name, folder: folder || '', notes: '',
      idle: { type: 'RainbowCircling', staticColour: { default: '#000000' }, pulseColour: { default: '#ffffff' } },
      notPressedColour: { default: '#000000' },
      pressedColour: { default: 'random' },
      holdMs: 0, fadeMs: 0, moves: [],
    };
  }

  const api = {
    INPUTS, DIRECTIONS, ATTACKS, COLOUR_SLOTS, IDLE_TYPES, WAVE_DIRECTIONS, WAVE_SPEEDS, NAMED_COLOURS, RANDOM_COLOURS,
    ANIMATIONS, LIMITS, SIZES, FLASH_LIMIT_BYTES,
    validate, generate, estimate, parseNativeClasses, findNativeClasses, nativeFolder, folderNameError, folderPathError, resolveColour, hexToRgb, colourName,
    splitProject, assembleProject, characterPath, characterKey, isCharacterFile, PROFILES_FILE, MOTIONS_FILE, PROJECT_DIR,
    mirrorMotion, mirrorMove, defaultAnimation, newCharacter,
  };

  if (typeof module !== 'undefined' && module.exports) module.exports = api;
  else root.KaimanaCodegen = api;

  // Command line: node editor/codegen.js [sketchFolder]
  if (typeof require !== 'undefined' && typeof module !== 'undefined' && require.main === module) {
    const fs = require('fs');
    const path = require('path');
    const dir = process.argv[2] || (fs.existsSync(PROJECT_DIR) || fs.existsSync('characters.json') ? '.' : path.join(__dirname, '..'));
    const read = rel => (fs.existsSync(path.join(dir, rel)) ? fs.readFileSync(path.join(dir, rel), 'utf8') : null);
    const characterFiles = [];
    const walkCharacters = rel => {
      if (!fs.existsSync(path.join(dir, rel))) return;
      for (const e of fs.readdirSync(path.join(dir, rel), { withFileTypes: true })) {
        const p = rel + '/' + e.name;
        if (e.isDirectory()) walkCharacters(p);
        else if (isCharacterFile(p)) characterFiles.push({ path: p, text: read(p) });
      }
    };
    walkCharacters(PROJECT_DIR);
    const project = assembleProject({ profiles: read(PROFILES_FILE), motions: read(MOTIONS_FILE), characters: characterFiles, legacy: read('characters.json') });
    project.warnings.forEach(w => console.warn('warning: ' + w));
    const doc = project.doc;
    if (!doc) process.exit(1);
    const headers = [{ path: 'Characters.h', text: fs.readFileSync(path.join(dir, 'Characters.h'), 'utf8') }];
    const walk = rel => {
      const full = path.join(dir, rel);
      if (!fs.existsSync(full)) return;
      for (const e of fs.readdirSync(full, { withFileTypes: true })) {
        const p = rel + '/' + e.name;
        if (e.isDirectory()) walk(p);
        else if (/\.(h|hpp|hh)$/i.test(e.name)) headers.push({ path: p, text: fs.readFileSync(path.join(dir, p), 'utf8') });
      }
    };
    walk('src');
    const result = generate(doc, { natives: findNativeClasses(headers) });
    if (result.errors.length) {
      result.errors.forEach(e => console.error('error: ' + e));
      process.exit(1);
    }
    fs.writeFileSync(path.join(dir, 'GeneratedCharacters.cpp'), result.code);
    const s = result.stats;
    console.log(`Wrote GeneratedCharacters.cpp: ${s.uniqueProfiles} unique profiles, ${s.counts.moves} moves, ${s.tableBytes} bytes of tables (estimated flash ${s.estimatedFlash}/${s.flashLimit})`);
  }
})(typeof window !== 'undefined' ? window : globalThis);
