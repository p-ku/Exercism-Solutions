class Matrix:
    def __init__(self, matrix_string):
        self.matrix = [line.split() for line in matrix_string.splitlines()]
        for i, line in enumerate(self.matrix):
            self.matrix[i] = [int(element) for element in line]

    def row(self, index):
        return self.matrix[index-1]

    def column(self, index):
        return [self.matrix[i[0]][index-1] for i in enumerate(self.matrix)]
