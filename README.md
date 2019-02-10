# Not-gate-Characterstic-curve
This project aims at verifying the characteristic curve of an logic inverter. 
The IC chosen for NOT GATE is of the “HC” sub family which can work at 3.3 V, consistent with logic high and low for MSP430 microcontroller.
The project is developed under the guidance of Mr D.V Gadre in CEDT lab at Netaji Subhas Institute of Technology (now Netaji Subhas University of Technology from Sept-2018).
The microcontroller generates an analog voltage through an  eight bit R-2R DAC ladder and this is used as an input to the NOT gate. 
The MSP then checks through an ADC the input and output voltage and send those via serial communication to python and the python code then plots them.
Its essential to use an op-amp non inverting unity gain amplifier in between the output of the DAC and the input to the NOT gate to avoid loading effects. 
The codeflow for the same is that first MSP and python wait for handshake signals for communication and then MSP one by one sends the data to the DAC ladder (waits for some time for the voltage across to be settled) and then reads the value from its ADC and send it via serial communication. 

One other way to generate an analog voltage could be through a PWM wave and applying a low pass filter at the output of the wave. The critical thing here is that the cut off frequency of the low pass filter should be very less than the frequency of the PWM wave. But we must have an accurate clock too i.e use a crystal to generate a clock. In MSP430G2553 the DCO module sets the clock frequency which is not often stable and accurate and this specific version of MSP430 device does not have a feature to include a high frequency crystal so using an R-2R DAC ladder seemed like the better alternative.
It’s still an ongoing project and the curve needs a mathematical equation to fit the data points and once determined this will be converted into a stand alone hardware.  
