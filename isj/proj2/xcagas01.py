#!/usr/bin/env python3

class Polynomial:
    '''
    Class utilised for containing polynomials.

    This class includes __init__, __str__, __add__ and __pow__ magic methods, along with derivative and at_value methods.

    These methods describe usual mathematical behavior of polynomials.
    '''
     
    def __init__(self, *args, **kwargs):
        '''
        A new object of the Polynomial class can be created using three types of syntax:
        
        >>> pol1 = Polynomial([1,-3,0,2])
        >>> pol1 = Polynomial(1,-3,0,2)
        >>> pol1 = Polynomial(x0=1,x3=2,x1=-3)
        '''
        if len(kwargs) == 0:
            if isinstance(args[0], list):
                args = args[0]
            self.polynom = list(args)
        else:
            lastCoef = sorted(kwargs)[-1]
            lastCoefNum = int(lastCoef[1:])
            self.polynom = [0]*(lastCoefNum+1)
            for key in kwargs:
                coefNum = int(key[1:])
                self.polynom[coefNum] = kwargs[key]

    def __str__(self):
        '''
        Returns a printable string of a polynomial in usual manner:
        a*x^n + b*x^n-1 + c*x^n-2 + ... + m*x + n
        '''
        print_str = ''
        for index, value in enumerate(self.polynom):
            if index == 0:
                if value < 0:
                    print_str = '- ' + str(abs(value))
                else:
                    print_str = '+ ' + str(value)
            elif index == 1:
                if value < 0:
                    print_str = '- ' + str(abs(value)) + 'x ' + print_str
                elif value > 0:
                    print_str = '+ ' + str(value) + 'x ' + print_str
            else:
                if value < 0:
                    print_str = '- ' + str(abs(value)) + 'x^' + str(index) + ' ' + print_str
                elif value > 0:
                    print_str = '+ ' + str(value) + 'x^' + str(index) + ' ' + print_str
        if print_str[:2] == '+ ' or print_str[:2] == '- ':
            print_str = print_str[2:]
        return print_str

    def __add__(self, other):
        '''
        Acepts two polynomials and returns a sum in form of a new Polynomial.

        The original polynomial is not changed. If you wish to save the result use 'pol1 = pol1 + pol2' in a typical manner.
        '''
        length1 = len(self.polynom)
        length2 = len(other.polynom)
        result = [0]*max(length1, length2)
        for i in range(min(length1, length2)):
            result[i] = self.polynom[i]+other.polynom[i]
        if length1 < length2:
            for i in range(length2-length1):
                result[i + length1]=other.polynom[i + length1]
        else:
            for i in range(length1-length2):
                result[i + length2]=self.polynom[i + length2]
        return Polynomial(result)

    def __pow__(self, n):
        '''
        Calculates a polynomial to a power of n and returns a polynomial.

        The original polynomial is not changed. If you wish to save the result use 'pol1 = pol1 ** n' in a typical manner.
        '''
        length = len(self.polynom)
        result = [0]*((length-1)*n+1)
        for index, coeff in enumerate(self.polynom):
            result[index] = coeff
        for i in range(n-1):
            for index1, coeff1 in enumerate(result):
                if index1 <= (length-1)*(i+1):
                    for index2, coeff2 in enumerate(self.polynom):
                        result[index1 + index2] += coeff1 * coeff2
        return Polynomial(result)

    def derivative(self):
        '''
        Accepts a polynomial and returns its derivative.
        '''
        lenght = len(self.polynom)
        derivative = [0]*(lenght - 1)
        for i in range(lenght - 1):
            derivative[i] = (i + 1)*self.polynom[i + 1]
        return Polynomial(derivative)

    def at_value(self, *args):
        '''
        Calculates the value of a polynomial by substituting for x and returns it.

        If two parametes are enetred, returns an absolute value of subrtraction of the seond from the first.
        '''
        if len(args) == 1:
            atValue = 0
            for index, value in enumerate(self.polynom):
                atValue += value * (args[0] ** index)
            return atValue
        elif len(args) == 2:
            atValue1 = 0
            atValue2 = 0
            for index, value in enumerate(self.polynom):
                atValue1 += value * (args[0] ** index)
            for index, value in enumerate(self.polynom):
                atValue2 += value * (args[1] ** index)
            return abs(atValue1 - atValue2)
        else:
            raise ValueError('Invalid count of arguments. Function "at_value" accepts 1 to 2 arguments.')
