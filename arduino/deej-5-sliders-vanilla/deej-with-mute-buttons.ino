
#include <OneButton.h>

const int NUM_SLIDERS = 5;
const int sliderInputs[NUM_SLIDERS] = {A0, A1, A2, A3, A4};
const int buttonInputs[NUM_SLIDERS] = {
    12,
    10,
    8,
    7,
    4,
};
const bool inverseMuteValue = true;

OneButton buttons[NUM_SLIDERS];

// Argument 2 = Arduino pin number (most are valid)

int analogSliderValues[NUM_SLIDERS];

bool isMuted[NUM_SLIDERS];

unsigned long previousMillis = 0; // will store last Time slider states were sent
const long sendInterval = 50;

void setup()
{

    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        pinMode(sliderInputs[i], INPUT);
        buttons[i] = OneButton(
            buttonInputs[i], // Input pin for the button
            true,            // Button is active LOW
            true             // Enable internal pull-up resistor
        );
        buttons[i].attachClick(toggleMute, i);
        isMuted[i] = false;
    }

    Serial.begin(9600);
}

void loop()
{
    tickButtons();
    updateSliderValues();

    sendSliderValues(); // Actually send data (all the time)
}

void updateSliderValues()
{
    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        analogSliderValues[i] = analogRead(sliderInputs[i]);
    }
}

void tickButtons()
{
    for (int i = 0; i < NUM_SLIDERS; i++)
    {
        buttons[i].tick();
    }
}

void toggleMute(int i)
{
    isMuted[i] = !isMuted[i];
}

void sendSliderValues()
{

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sendInterval)
    {
        previousMillis = currentMillis;

        String builtString = String("");

        for (int i = 0; i < NUM_SLIDERS; i++)
        {
            int value = (int)analogSliderValues[i];

            if (isMuted[i])
            {
                value = inverseMuteValue ? 1023 : 0;
            }

            builtString += String(value);

            if (i < NUM_SLIDERS - 1)
            {
                builtString += String("|");
            }
        }

        Serial.println(builtString);
    }
}
