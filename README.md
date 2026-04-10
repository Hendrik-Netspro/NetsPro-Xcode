# NetSpectre Pro (C++ Edition)

NetSpectre Pro is a terminal-like CLI application written in C++. It provides a simple command shell with optional developer mode and an "AkteViewer" for working with encrypted `.persondata` files.

> Note: the project currently uses a custom letter-shift cipher for the AkteViewer data. This is **not** cryptographically secure and should be treated as obfuscation.

## Access / Login

**Credentials are not public.** If you need access, request a username + password from:

**Hendrik Hanking** — `hendrik.hanking@icloud.com`

## Commands

### User commands

- `info` — show version + serial number + dev mode
- `cls` / `clear` — clear the terminal
- `akte` — interactive mode: open a file (or create a new one)
- `akte neu` — create a new `.persondata` file and open it
- `akte open <Name>` — open `<Name>.persondata`
- `jarvis-serve` — start the JARVIS chat loop
- `help` — show commands
- `exit` — exit program

### Developer commands

- `devinfo` — developer status + user
- `devmode on|off` — toggle dev mode (developer-only)

## Building

If you prefer to build the programm yourself:
Requires a C++17 compiler.

```bash
g++ -std=c++17 -O2 -o netspectre main.cpp
```

### Platform notes

- **Windows**: uses PowerShell to read the BIOS serial number
- **macOS**: uses `system_profiler`
- **Linux**: reads `/sys/class/dmi/id/product_serial`

## Data format / AkteViewer

- Files are stored as encrypted text with the shift pattern `{9,9,4,13,2,10,3}`
- `akte neu` creates a file with a generated key and basic name metadata
- `akte open` decrypts, displays content, and lets you append lines; an empty line saves and exits

## Troubleshooting

- If the connection/Internet behaves weird (e.g. GitHub glitches), wait a bit and try again.
- Linux: you may need permission to read `/sys/class/dmi/id/product_serial`.

## License / copyright

COPYRIGHT Hendrik Vincent Hanking 2025. All rights reserved.
