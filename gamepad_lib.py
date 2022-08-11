from inputs import get_gamepad

class Gamepad:
    """class for gamepad controll"""

    def __init__(self):
        self.R_trig = 0
        self.L_trig = 0
        self.R_analog_x = 0
        self.R_analog_y = 0
        self.L_analog_x = 0
        self.L_analog_y = 0

    def update(self):
        """method responsible for actualisation of gamepad data"""
        events = get_gamepad()
        for event in events:
            if event.ev_type == "Absolute":
                if event.code == "ABS_Y":
                    self.L_analog_y = event.state
                elif event.code == "ABS_X":
                    self.L_analog_x = event.state
                elif event.code == "ABS_RY":
                    self.R_analog_y = event.state
                elif event.code == "ABS_RX":
                    self.R_analog_x = event.state
                elif event.code == "ABS_Z":
                    self.L_trig = event.state
                elif event.code == "ABS_RZ":
                    self.R_trig = event.state
                else:
                    pass
                

