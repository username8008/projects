# ESP32 Optical Data Transmission System

A working **proof-of-concept prototype** for optical data communication, built with **two ESP32 boards**, LEDs, photoresistors (LDRs), **ESP-IDF (C)**, **FreeRTOS**, and a **C# WinForms** desktop application. The current version focuses on reliable short-text transmission; the longer-term goal is to develop a faster optical link capable of transferring images.

The application sends text over TCP/IP to a transmitter ESP32. The transmitter encodes each byte as a 16-bit frame (8 data bits + 8 CRC bits) and sends it over an LED. A second ESP32 measures the incoming light using its ADC, validates the CRC-8, and prints the reconstructed text over its serial console. A separate optical signal from the receiver indicates that it is ready for the next character.

> **Project status:** Early-stage, working hardware prototype. This is a proof of concept, not a production-ready optical communication system. The photodiode/laser upgrade and image transmission described below are **planned, not yet implemented**.

## Demo


🎥 **Video demonstration:** Coming soon.



## Architecture

```text
C# WinForms desktop application
              |
              | Wi-Fi / TCP (port 8008)
              v
ESP32 transmitter (FreeRTOS TCP server + message queue)
              |
              | GPIO 25 -> LED -> optical data channel
              v
       LDR -> ADC (GPIO 35)
         ESP32 receiver
              |
              v
    CRC validation + serial output

Reverse optical ready signal:
ESP32 receiver GPIO 26 -> LED -> LDR -> transmitter ADC (GPIO 35)
```

## Features

- ESP32 firmware implemented in C using ESP-IDF.
- FreeRTOS tasks and a queue separating TCP message reception from optical transmission.
- TCP server on port **8008**, with a Windows Forms client written in C#.
- Bit-by-bit LED transmission and ADC-based optical reception.
- **CRC-8** error detection using polynomial `0x07`.
- Optical **receiver-ready signaling** before sending each character.
- Serial-console output of decoded data at the receiver.

## Hardware and pin assignments

Two ESP32 development boards, two LEDs, two photoresistors (LDRs), appropriate current-limiting and divider resistors, and connecting wires are used in the prototype.

| Device | GPIO | Role |
| --- | --- | --- |
| Transmitter ESP32 | 25 | Optical data LED output |
| Transmitter ESP32 | 35 (ADC1 channel 7) | Receiver-ready light sensor |
| Receiver ESP32 | 35 (ADC1 channel 7) | Optical data light sensor |
| Receiver ESP32 | 26 | Receiver-ready LED output |

**Important:** Refer to your own circuit wiring before powering the boards. Use suitable LED current-limiting resistors and ensure the ADC input does not exceed the board's allowed voltage.

The ADC decision thresholds are currently hard-coded (`2000` at the transmitter and `2400` at the receiver, on a 12-bit scale) and may need adjustment for ambient lighting and sensor placement.

## Communication protocol

Each transmitted byte is encoded into a 16-bit word:

```text
+----------------------+----------------------+
|     Data byte        |       CRC-8          |
|       8 bits         |       8 bits         |
+----------------------+----------------------+
    First on wire           Last on wire
```

- Bits are transmitted most-significant bit first.
- The CRC uses polynomial `0x07` with an initial value of `0x00`.
- The receiver verifies the 16-bit word by computing the CRC remainder. If validation fails, it outputs `#` as an error marker.
- A newline (`\n`, ASCII 10) is used as an end-of-message marker.
- The receiver uses its LED to indicate readiness, which the transmitter detects through an LDR before each 16-bit frame.
- The prototype uses an approximately 100 ms delay per transmitted bit, so throughput is low and additional synchronization overhead applies.

This prototype uses an optical data channel in one direction and an optical ready signal in the reverse direction; it does **not** support arbitrary two-way text messaging.

## Repository structure

```text
opticFiberEsp/
├── ver1.0/                         # Earlier Arduino-based prototype
├── ver2.0/
│   ├── transmitter/ESP_IDF_PROJ/   # ESP-IDF transmitter firmware
│   └── reciver/ESP_IDF_PROJ/       # ESP-IDF receiver firmware
├── front for transmitter/
│   └── msgSender/                 # C# WinForms TCP client (.NET Framework 4.7.2)
└── README.md
```

*Folder spelling (`reciver`) is preserved to match the current repository.*

## Building and running

### 1. ESP32 firmware

Install ESP-IDF and set up its environment. From each ESP-IDF project directory, build and flash the firmware for your particular ESP32 target and serial port:

```bash
idf.py set-target esp32
idf.py build
idf.py -p YOUR_SERIAL_PORT flash monitor
```

The transmitter also uses Espressif's `protocol_examples_common` helper for network setup. Ensure the matching ESP-IDF examples/common component is available in your build environment.

### 2. Configure Wi-Fi (transmitter)

Run inside `ver2.0/transmitter/ESP_IDF_PROJ/`:

```bash
idf.py menuconfig
```

Find the example network configuration (the values correspond to `CONFIG_EXAMPLE_WIFI_SSID` and `CONFIG_EXAMPLE_WIFI_PASSWORD`) and enter your own network name and password.

**Do not commit `sdkconfig`, `sdkconfig.old`, or any file containing your Wi-Fi credentials.** These files are ignored by the repository's `.gitignore`.

### 3. Desktop client

Open `front for transmitter/msgSender/msgSender.slnx` in a compatible Visual Studio version, or open the `.csproj` directly. The project targets **.NET Framework 4.7.2** and uses Windows Forms.

1. Connect the transmitter ESP32 to the Wi-Fi network.
2. Find its IP address from the ESP-IDF serial monitor or your router.
3. Open the desktop client, enter the ESP32 IP address, and confirm it.
4. Enter a message and send it.
5. Observe the receiver's decoded output in its serial monitor.

Use short ASCII messages for predictable results: the current firmware processes individual bytes and has a limited receiver buffer.

## Current limitations

- Low optical bit rate, intended as a proof of concept.
- LDR response and ambient lighting affect reliability and practical range.
- Fixed timing and ADC thresholds, rather than adaptive calibration.
- CRC detects errors but there is no automatic retransmission over the optical link.
- The current receive buffer supports only short messages.
- The desktop app sends UTF-8, but the optical receiver handles individual bytes rather than decoding multibyte characters.

## Roadmap: from text to image transmission

The current LED-and-LDR design demonstrates the basic communication protocol and hardware-software integration. The next goal is to increase the link bandwidth and extend the system beyond text messages.

1. **Upgrade the optical receiver:** Replace the LDRs with faster **photodiodes** and suitable signal-conditioning circuitry to support higher data rates.
2. **Upgrade the optical transmitter:** Explore replacing the LEDs with an appropriately rated **laser-based light source**, using suitable driving circuitry and observing laser eye-safety requirements.
3. **Improve timing and robustness:** Use more precise ESP32 timing mechanisms, refine optical framing/synchronization, and measure error rates under different conditions.
4. **Support image transmission:** Design packetization and reassembly for binary image data, with error handling suitable for longer transfers.

**These are future development goals, not features of the current prototype.**

## Technologies

**C · ESP-IDF · FreeRTOS · ESP32 · GPIO · ADC · TCP/IP · C# · Windows Forms · CRC-8**

## License

No license has been selected yet. Add a `LICENSE` file if you want to define reuse permissions.
