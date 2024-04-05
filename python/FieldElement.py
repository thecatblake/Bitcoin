import unittest


class FieldElement:
    def __init__(self, num, prime):
        self.num = num % prime
        self.prime = prime

    def __check_class(self, other):
        if self.prime != other.prime:
            raise TypeError("Cannot add two numbers in different field")

    def __eq__(self, other):
        return self.num == other.num and self.prime == other.prime

    def __add__(self, other):
        self.__check_class(other)
        return FieldElement((self.num + other.num) % self.prime, self.prime)

    def __sub__(self, other):
        self.__check_class(other)
        if -other.num > self.num:
            other.num = self.prime - (-other.num % self.prime)
        return FieldElement((self.num - other.num) % self.prime, self.prime)

    def __mul__(self, other):
        self.__check_class(other)
        return FieldElement((self.num * other.num) % self.prime, self.prime)

    def __pow__(self, exponent):
        n = exponent % (self.prime - 1)
        num = pow(self.num, n, self.prime)
        return FieldElement(num, self.prime)

    def __truediv__(self, other):
        self.__check_class(other)
        return self * (other ** (self.prime - 2))

    def __str__(self):
        return f"FieldElement({self.num}, {self.prime}"

    def __repr__(self):
        return self.__str__()


class FieldElementTest(unittest.TestCase):
    def test_equality(self):
        a = FieldElement(7, 13)
        b = FieldElement(6, 13)
        c = FieldElement(7, 17)
        d = FieldElement(6, 17)
        e = FieldElement(7, 13)

        self.assertFalse(a == b)
        self.assertFalse(a == c)
        self.assertFalse(a == d)
        self.assertTrue(a == e)

    def test_addition(self):
        a = FieldElement(7, 13)
        b = FieldElement(6, 13)

        self.assertEqual(a + b, FieldElement(0, 13))
        self.assertEqual(a - b, FieldElement(1, 13))
        self.assertEqual(b - a, FieldElement(12, 13))

    def test_multiplication(self):
        a = FieldElement(3, 31)
        b = FieldElement(24, 31)

        self.assertEqual(a * b, FieldElement(10, 31))
        self.assertEqual(a / b, FieldElement(4, 31))


if __name__ == '__main__':
    unittest.main()
