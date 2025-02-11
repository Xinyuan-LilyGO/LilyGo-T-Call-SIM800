# StreamDebugger

Just allows easier debugging of Serial-based communication on Arduino.

StreamDebugger class is an Arduino Stream, that dumps all data to another Stream for debug purposes.

## Usage:

```cpp
#include <StreamDebugger.h>
StreamDebugger StreamDbg(Serial1, Serial);

...

void setup() {
    // Setup debug stream
    Serial.begin(115200);
    delay(10);

    // Setup data stream
    Serial1.begin(9600);
    delay(10);

    // Now use StreamDbg instead of Serial1

    StreamDbg.println("Hello world!");
    // The message is sent to Serial1,
    // and automatically duplicated to Serial
}

void loop() {
    // Start direct-access from Serial to Serial1
    StreamDbg.directAccess();
}
```

