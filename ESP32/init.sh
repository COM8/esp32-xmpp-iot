#bin/bash

# Init submodules
git submodule init
git submodule update
git submodule update --init --recursive

# Include ESP32 Snippets `cpp_utils`
if [ -d /tmp/esp32-snippets ]; then
		rm -rf /tmp/esp32-snippets
fi
git clone git@github.com:COM8/esp32-snippets.git /tmp/esp32-snippets
if [ -d components ]; then
		rm -rf components
fi
mkdir components
cp -r /tmp/esp32-snippets/cpp_utils/ components/
rm -rf /tmp/esp32-snippets/
