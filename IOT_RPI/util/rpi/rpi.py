class RPI():

    def __init__(self, id, type):
        self.id = id
        self.type = type

    def __str__(self):
        return ('Id:\t\t' + str(self.id) +
                '\nType:\t\t' + self.type)
