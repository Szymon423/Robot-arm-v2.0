class Point:
    """class responsible for representing points coordinates"""
    # cartesian coordinates
    x = 0.
    y = 0.
    z = 0.
    eps = 0.


    # joint angles in radians
    lam1 = 0.
    lam2 = 0.
    lam3 = 0.
    lam4 = 0.


    def __init__(self, x, y, z, e):
        """constructor"""
        self.x = x
        self.y = y
        self.z = z
        self.eps = e
