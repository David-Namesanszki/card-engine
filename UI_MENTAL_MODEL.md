# UI Mental Model

## Layers

```
Battle (core logic)
    │  events
    ▼
BattleUI (orchestrator)
    │  owns & drives
    ▼
HandUI / DrawPileUI / ... (sub-UIs)
    │  own & manage
    ▼
RenderObject (display data)
    │  drawn by
    ▼
Renderer
```

## Responsibilities

**Battle** — pure game logic, no visuals. Emits events.

**BattleUI** — top-level bridge between core and UI.
- Subscribes to Battle events
- Knows card data (type, texture, etc.)
- Creates `CardUI` visuals
- Decides *which* animation factory to use (based on card type)
- Delegates *how* to display to sub-UIs

**HandUI / sub-UIs** — self-contained inner policy.
- Is a `RenderObject` (has its own background, position, drawn by renderer)
- Owns child `RenderObject`s (slots, card visuals)
- Does not know about card types or game state
- Exposes a simple interface: `addCard(CardUI, Animation)`
- Manages layout (bezier arc), slot assignment, animation triggering internally

**RenderObject** — pure display data (struct). Texture, position, size, rotation, scale, color, layer. Never contains logic.

**Slot** — anchor struct (not a RenderObject). Defines the transform + style properties (position, rotation, scale, tint) that arriving objects inherit. Never passed to the renderer.

## Animation System

An `Animation` drives one or more **float channels** simultaneously. Each channel has a `from`, `to`, and a setter function. The `Animator` knows nothing about what it is animating — only the setters do.

```
AnimationConfig  →  duration + easing (stored, reusable)
Channel          →  float from/to + setter (what property, which object)
Animation        →  config + channels + tag + onComplete
```

**Factories** — functions that combine an `AnimationConfig` with dynamic parameters (which card, which slot) to produce a ready-to-run `Animation`. The shape of the animation is defined once; dynamic values flow in as arguments.

Multiple factories can exist for the same event — `BattleUI` picks the right one based on card type. `HandUI` never knows which was used.

## Key Boundaries

| Who knows what | Battle | BattleUI | HandUI | Animator |
|---|---|---|---|---|
| Game state | ✓ | ✓ (read) | ✗ | ✗ |
| Card types | ✓ | ✓ | ✗ | ✗ |
| Layout / bezier | ✗ | ✗ | ✓ | ✗ |
| What is animated | ✗ | ✗ | ✓ | ✗ |
| How animation runs | ✗ | ✗ | ✗ | ✓ |
