# Architecture

## Overview

The project separates VBE mode discovery from VBE mode selection.

The selection algorithm is platform-independent and can be compiled
and tested as a standard C17 program.

## Components

### VBE Discovery Layer

Responsible for:

- Calling BIOS interrupt 0x10.
- Requesting VBE controller information (AX = 0x4F00).
- Enumerating supported video modes.
- Requesting mode information (AX = 0x4F01).
- Translating BIOS structures into VbeMode entries.

### Selection Layer

Implemented in `src/vbe_mode.c`.

Responsible for:

- Validating the requested configuration.
- Filtering unsupported video modes.
- Finding exact matches.
- Comparing resolution differences.
- Comparing color-depth differences.
- Returning the selected mode number.

### VBE Mode Activation Layer

Responsible for activating the selected mode through
the VBE BIOS interface (AX = 0x4F02).

## Selection Strategy

1. Reject invalid requests.
2. Filter modes without linear framebuffer support.
3. Accept only packed-pixel and direct-color memory models.
4. Return an exact match immediately.
5. Minimize pixel-count difference.
6. Use color depth as a tie-breaker.
7. Fall back to the legacy default mode.

## Complexity

Let N be the number of available video modes.

Time complexity: O(N)

Auxiliary space complexity: O(1)
