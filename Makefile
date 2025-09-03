TARGET = tesla_lux
BUILD = build
SOURCES = source/main.cpp

INCLUDES = -Ilib/libtesla_precompiled/include
LIBS = lib/libtesla_precompiled/libtesla.a -lnx

CXX = aarch64-none-elf-g++
CXXFLAGS = -O2 -fno-exceptions -fno-rtti -march=armv8-a -mtune=cortex-a57
LDFLAGS = -Wl,--allow-multiple-definition

all: $(BUILD)/$(TARGET).ovl

$(BUILD)/$(TARGET).ovl: $(SOURCES)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BUILD)/$(TARGET).elf $(SOURCES) $(LIBS) $(LDFLAGS)
	$(CXX) -O binary $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).ovl

clean:
	rm -rf $(BUILD)

.PHONY: all clean
