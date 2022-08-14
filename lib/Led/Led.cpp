# include <Arduino.h>
# include <Led.h>

Led::Led(byte red_pin, byte green_pin, byte blue_pin) {
    this->red_pin = red_pin;
    this->green_pin = green_pin;
    this->blue_pin = blue_pin;
    this->effect = LedEffect::bright;
    this->color = LedColor::no_color;
    this->oldMillis = millis();
    this->brightness = 0; 
    this->last_refresh_time = 0;
}

void Led::update() {
    switch (this->effect) {
        case LedEffect::bright:
            this->brightness = 100;
            bright();
            break;
        case LedEffect::dim:
            this->brightness = 0;
            bright();
            break;
        case LedEffect::fast_pulse:
            pulse(600);
            break;
        case LedEffect::slow_pulse:
            pulse(1200);
            break;
    }
}

void Led::setEffect(LedEffect effect) {
    this->effect = effect;
}

void Led::setColor(LedColor color) {
    this->color = color;
}

void Led::pulse(int period) {
    if((last_refresh_time + 1) < millis() ){
        last_refresh_time = millis();
        // https://thingpulse.com/breathing-leds-cracking-the-algorithm-behind-our-breathing-pattern/
        // https://github.com/ThingPulse/esp32-icon64-a2dp/blob/master/src/main.cpp#L176-L211
        float min =  0.381966011;
        float amplitude = 49.0; //42.54590641;
        uint8_t b = (exp(sin(millis()/(float)period*PI)) - min)* amplitude;
        this->brightness = constrain((byte)b * 255 / 100, x, 255);
        Serial.print(0);
        Serial.print(" ");
        Serial.print(255);
        Serial.print(" ");
        Serial.print(127);
        Serial.print(" ");
        Serial.println(255 - this->brightness);

        byte red_brightness = (mapRed(this->color) * this->brightness) / 100;
        byte green_brightness = (mapGreen(this->color) * this->brightness) / 100;
        byte blue_brightness = (mapBlue(this->color) * this->brightness) / 100;

        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
    }
}

void Led::blink(int period) {
    if (millis() - last_refresh_time > period ){
        this->brightness = 100;

        byte red_brightness = (mapRed(this->color) * this->brightness) / 100;
        byte green_brightness = (mapGreen(this->color) * this->brightness) / 100;
        byte blue_brightness = (mapBlue(this->color) * this->brightness) / 100;

        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
    }
    if (millis() - last_refresh_time > period*2) {
        this->brightness = 25;

        byte red_brightness = mapRed(this->color) * this->brightness / 100;
        byte green_brightness = mapGreen(this->color) * this->brightness / 100;
        byte blue_brightness = mapBlue(this->color) * this->brightness / 100;

        invertAnalogWrite(this->red_pin, red_brightness);
        invertAnalogWrite(this->green_pin, green_brightness);
        invertAnalogWrite(this->blue_pin, blue_brightness);
        last_refresh_time = millis();
    }
}


void Led::invertAnalogWrite(int pin, int value) {
    analogWrite(pin, 255 - value);
}

void Led::bright() {
    invertAnalogWrite(this->red_pin, this->brightness);
}

byte Led::mapRed(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 255;
        case LedColor::green:
            return 0;
        case LedColor::blue:
            return 0;
        case LedColor::yellow:
            return 255;
        case LedColor::purple:
            return 255;
        case LedColor::cyan:
            return 0;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}

byte Led::mapGreen(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 0;
        case LedColor::green:
            return 255;
        case LedColor::blue:
            return 0;
        case LedColor::yellow:
            return 255;
        case LedColor::purple:
            return 0;
        case LedColor::cyan:
            return 255;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}

byte Led::mapBlue(LedColor color) {
    switch (color) {
        case LedColor::red:
            return 0;
        case LedColor::green:
            return 0;
        case LedColor::blue:
            return 255;
        case LedColor::yellow:
            return 0;
        case LedColor::purple:
            return 255;
        case LedColor::cyan:
            return 255;
        case LedColor::white:
            return 255;
        default:
            return 0;
    }
}
