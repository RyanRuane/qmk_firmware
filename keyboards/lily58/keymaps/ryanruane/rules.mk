# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AUDIO_ENABLE = no           # Audio output
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
COMMAND_ENABLE = no         # Commands for debug and configuration
CONSOLE_ENABLE = no         # Console for debug
ENCODER_ENABLE = no
EXTRAKEY_ENABLE = yes       # Audio control and System control
MOUSEKEY_ENABLE = no        # Mouse keys
NKRO_ENABLE = yes
OLED_ENABLE= yes            # OLED display
RGBLIGHT_ENABLE = no        # Enable WS2812 RGB underlight.
RGB_MATRIX_ENABLE = no
STENO_ENABLE = yes          # Enable steno protocol for plover
STENO_PROTOCOL = all        # Allow protocol switching (keycodes: QK_STENO_BOLT, QK_STENO_GEMINI)
SWAP_HANDS_ENABLE = no      # Enable one-hand typing
TAP_DANCE_ENABLE = yes      # Enable tap dance functionality

# If you want to change the display of OLED, you need to change here
SRC +=  oled/layer_state.c \
		oled/keyboard_logo.c \
        oled/clap_logo.c \
        oled/keylogger.c \
