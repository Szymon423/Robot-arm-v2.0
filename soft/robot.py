from math import gamma
from random import betavariate
import numpy as np
from point import Point


class Robot:
    """class responsible for handlong robot controll"""

    # lengths of particular robot joints
    d1 = 0.1
    d2 = 0.5
    d3 = 0.4
    d4 = 0.15

    # logic permisions
    drivesOk = [False, False, False, False]
    homingDone = False

    def __init__(self, d1, d2, d3, d4):
        """initialisation of robot object"""
        self.d1 = d1
        self.d2 = d2
        self.d3 = d3
        self.d4 = d4
        # to do
        pass


    def initialiseDrives(self):
        """method responsible for initialising all drives - necessary for further actions"""
        # to do - checking communication with each drive
        # it has to be sth like: send broadcast info to all drives maybe "WhoAreYou"
        # and wait for response of each of them. They have to introduce themselves by responding
        # with proper number 1-4, or name. Then I can assign proper angle to corresponding motor.
        # After this command User has to perform homing sequention
        # still need to implement identification in mcu
        pass


    def calculateInversKinematics (self, p):
        """method for calculating desired kartesian coordinates basing on angles"""
        
        p.x = np.cos(p.lam4) * (self.d1 * np.cos(p.lam1 - np.pi / 2) + self.d2 * np.cos(p.lam1 + p.lam2 + np.pi / 2) + self.d3 * np.sin(p.lam1 + p.lam2 + p.lam3))
        p.y = np.sin(p.lam4) * (self.d1 * np.cos(p.lam1 - np.pi / 2) + self.d2 * np.cos(p.lam1 + p.lam2 + np.pi / 2) + self.d3 * np.sin(p.lam1 + p.lam2 + p.lam3))
        p.z = self.d1 * np.sin(self.lam1 - np.pi /2) + self.d2 * np.sin(p.lam1 + p.lam2 + np.pi / 2) + self.d3 * np.cos(p.lam1 + p.lam2 + p.lam3) + self.d4
        p.eps = np.pi / 2 - p.lam1 - p.lam2 - p.lam3


    def calculateInversKinematics (self, p):
        """method for calculating angles basing on desired kartesian coordinates"""
        
        # side calculations
        R = np.sqrt(np.power(p.x, 2) + np.power(p.y, 2))
        p = R - self.d3 * np.cos(p.eps)
        h = p.z - self.d4 + self.d3 * np.sin(p.eps)
        m = np.sqrt(np.power(m, 2) + np.power(h, 2))
        alfa = np.arctan2(h / p)
        beta = np.arccos((np.power(self.d1, 2) + np.power(m, 2) - np.power(self.d2, 2)) / (2 * m * self.d1))
        gamma = np.arccos((np.power(self.d1, 2) + np.power(self.d2, 2) - np.power(m, 2)) / (2 * self.d1 * self.d2))
        sigma = np.pi - beta - gamma
        fi = np.pi / 2 - alfa
        ro = np.pi / 2 - p.eps

        # saving calculated angles
        p.lam1 = alfa + beta - gamma
        p.lam2 = gamma
        p.lam3 = sigma + fi + ro
        p.lam4 = np.arctan2(p.y / p.x)

    
    def findHomePosition (self):
        """method for finding home position of every drive"""
        # to do - homing sequention should be done in following order
        #   drive nr. 1
        #   drive nr. 4
        #   drive nr. 3
        #   drive nr. 2
        # At the very beginning absolute position of every drive  
        pass




