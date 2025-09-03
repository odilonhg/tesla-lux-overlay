# README - Tesla Lux Overlay (Switch OLED)

**Projet** : Tesla Overlay qui affiche le niveau de luminosité (lux) en direct dans le menu Tesla.

**But** : Fournir un overlay `.ovl` compatible Tesla (Switch OLED incluse) qui lit le capteur de luminosité ambiante via les services système (libnx/setsys) et affiche la valeur en direct.

---

## Fichiers inclus

- `source/main.cpp`        -> Code principal (C++ / libtesla)
- `Makefile`               -> Makefile minimal pour devkitPro / libnx
- `romfs/icon.png`         -> (placeholder) icône pour l'overlay si tu veux l'inclure
- `README`                 -> Ceci

---

## Prérequis

1. devkitPro + libnx installés et fonctionnels.
2. libtesla (libtesla/tsl) installée et disponible pour lier l'overlay Tesla. Si tu ne l'as pas, récupère-la depuis le dépôt officiel de libtesla et installe les headers/libs.
3. Un Switch en CFW (Atmosphere) et Tesla Overlay installé (ça va de soi).

> NOTE : J'ai fait le maximum pour que le code compile "tel quel", mais selon ta version de libtesla ou libnx les noms d'API peuvent légèrement varier. Si y a une erreur de compilation, dis‑moi l'erreur et je t'ajuste ça, tabarnac.

---

## Instructions de build (général)

1. Mets-toi dans le dossier du projet.
2. `make` (le Makefile utilise les variables classiques de devkitPro).
3. Si la compilation réussit tu auras un fichier `.ovl` dans `build/`.
4. Copie le `.ovl` sur ta SD dans le dossier des overlays Tesla (ex. `sd:/tesla/overlays/` ou le dossier configuré par ton Tesla). Si t'es pas sûr du chemin, vérifie la doc de ton Tesla overlay.
5. Lance ta Switch, ouvre le menu Tesla — tu devrais voir une entrée « Capteur de Lumière » ou similaire.

---

## Comportement

- L'overlay lit périodiquement le lux via `setsysGetAmbientLightSensorLux()` (ou l'API disponible) et met à jour l'affichage.
- Si l'API n'est pas disponible la valeur affichée sera "N/A" et un message d'erreur sera loggé.

---

## `source/main.cpp`

```cpp
// main.cpp

#include <switch.h>
#include <string>
#include <memory>
#include <cstdio>

// Inclure les headers de libtesla. Selon l'install cela peut être "tesla.hpp" ou "tsl/tsl.hpp"
#include <tesla.hpp>

using namespace tsl;

class LuxGui : public Gui {
public:
    LuxGui() {}

    virtual Element* createUI() override {
        auto frame = new OverlayFrame("Capteur de Lumière", "Homebrew Dylan");

        auto list = new List();
        list->addItem(new CategoryHeader("Valeur actuelle (lux)"));

        luxLabel = new Text("Chargement...");
        list->addItem(luxLabel);

        frame->setContent(list);

        // update callback every frame (tesla framework assure un delta raisonnable)
        this->setUpdateCallback([this](Gui* gui) {
            float lux = 0.0f;
            Result rc = setsysGetAmbientLightSensorLux(&lux);
            if (R_SUCCEEDED(rc)) {
                char buf[64];
                snprintf(buf, sizeof(buf), "%.2f lux", lux);
                luxLabel->setText(buf);
            } else {
                // Si l'API exact n'existe pas sur certaines firmwares, on affiche N/A
                luxLabel->setText("N/A (API setsys manquante)");
            }
        });

        return frame;
    }

private:
    Text* luxLabel;
};

class LuxOverlay : public Overlay {
public:
    virtual void initServices() override {
        setsysInitialize();
    }

    virtual void exitServices() override {
        setsysExit();
    }

    virtual std::unique_ptr<Gui> loadInitialGui() override {
        return initially< LuxGui >();
    }
};

int main(int argc, char* argv[]) {
    // Initialisation basique libnx/tesla
    romfsInit();
    setsysInitialize();

    // Lance la boucle Tesla
    int ret = tsl::loop< LuxOverlay >(argc, argv);

    setsysExit();
    romfsExit();
    return ret;
}
```

> Remarques sur le code :
> - J'utilise `setsysGetAmbientLightSensorLux(&lux)` comme appel attendu. Sur certaines versions de libnx/setsys le nom exact peut différer ; si `setsysGetAmbientLightSensorLux` n'existe pas tu devras remplacer par l'appel disponible (ex : `setsysGetAmbientSensorLux` ou autre). Si tu me donnes l'erreur de compilation exacte, je te corrige ça rapido.

---

## Makefile (exemple simple)

```makefile
# Makefile minimal pour devkitPro + libnx + libtesla

TARGET = tesla_lux
BUILD = build
SOURCES = source/main.cpp

INCLUDES = -I$(DEVKITPRO)/libnx/include -I$(DEVKITPRO)/libtesla/include
LIBS = -L$(DEVKITPRO)/libnx/lib -lnx -L$(DEVKITPRO)/libtesla/lib -ltesla

CXX = $(DEVKITPRO)/tools/bin/aarch64-none-elf-g++
CXXFLAGS = -O2 -fno-exceptions -fno-rtti -march=armv8-a -mtune=cortex-a57
LDFLAGS = -Wl,--allow-multiple-definition

all: $(BUILD)/$(TARGET).ovl

$(BUILD)/$(TARGET).ovl: $(SOURCES)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD)/$(TARGET).elf $(SOURCES) $(LIBS) $(LDFLAGS)
	# Conversion ELF -> OVL (outil fourni par libtesla/devkit) ; si tu as mkovl ou script, replace la ligne suivante
	$(DEVKITPRO)/tools/bin/objcopy -O binary $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).ovl

clean:
	rm -rf $(BUILD)

.PHONY: all clean
```

> NOTE: Ce Makefile est volontairement minimal et peut nécessiter des ajustements selon ton environnement devkitPro (noms de toolchain, emplacement des libs, et la façon dont tu génères une `.ovl`). Si tu utilises `tesla-build` ou un exemple officiel de libtesla, préfère leur Makefile et remplace `main.cpp` par le code fourni.

---

## Dépannage rapide

- **Erreur : symbole setsysGetAmbientLightSensorLux introuvable** → Vérifie ta version de libnx/setsys. Cherche une fonction similaire (`setsysGetAmbientLightSensor...`). Donne-moi l'erreur complète et j'adapte.
- **Erreur de linking `-ltesla` introuvable** → Assure-toi d'avoir compilé/installé libtesla et que le chemin vers les libs est correct.
- **Overlay absent dans Tesla** → Vérifie le chemin d'installation sur la SD et que Tesla recharge la liste des overlays (parfois un reboot est nécessaire).

---

## Licence

Utilise comme tu veux, c'est open pour toi. Je fournis ça sans garantie – teste en connaissance de cause.

---

Si tu veux, je t'ajuste le Makefile pour ta configuration exacte (chemin `DEVKITPRO`, nom de toolchain) et je peux générer une version alternative qui utilise CMake si t'es plus à l'aise avec ça.

Allez, balance‑moi les erreurs si t'en as, j'te reprends ça vite, osti.
