# 🚀 Tesla Lux Overlay (Switch OLED)

**Realtime Ambient Light Sensor Display** – A Tesla Overlay plugin for Nintendo Switch that shows the console’s lux value live, right in the Tesla menu. Works on all models, including the Switch OLED.

---

## 🌟 Features

- Real-time display of ambient light (lux) readings.
- Works inside the Tesla Overlay menu.
- Lightweight, minimal, and efficient.
- Supports all Nintendo Switch models (OLED, v1-v2).
- Perfect for devs, testers, or sensor geeks.

---

## 🛠 Requirements

- Nintendo Switch running **CFW (Atmosphere)**.
- **Tesla Overlay** installed.
- **devkitPro** + **libnx** + **libtesla** for building.

---

## ⚡ Installation

1. **Install**: Copy the `.ovl` to your Tesla overlays folder:  
```
sd:/switch/.overlays/
```  
   (or `sd:/tesla/overlays/` depending on your setup)

3. **Run**: Open Tesla Overlay (`L + Down + Right Stick`) and select **Light Sensor**.

---

## 🖥 Screenshot

> ![Screenshot](romfs/icon.png)  
> Real-time lux reading inside Tesla Overlay.

---

## 🔧 Build Instructions

1. Clone the repo:  
   ```bash
   git clone <repo-url>
   cd tesla-lux-overlay
   ```

2. Ensure `devkitPro`, `libnx`, and `libtesla` are installed.

3. Compile the overlay:  
   ```bash
   make
   ```

> If you want automated builds, check out the included GitHub Actions workflow for CI `.ovl` generation.

---

## 📝 Usage

- Open Tesla Overlay.  
- Navigate to **Light Sensor**.  
- Observe the lux value updating in real-time.

---

## ⚠ Troubleshooting

- **`setsysGetAmbientLightSensorLux` not found** → Your libnx version may differ. Check `setsys.h` for the correct function.
- **Linking `-ltesla` fails** → Make sure libtesla is compiled and installed correctly.
- **Overlay doesn’t appear** → Check the overlay folder and restart your Switch.

---

## 📜 License

Open-source, free to use at your own risk. Modify, experiment, and have fun with Tesla overlays!
