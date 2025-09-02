#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>
using namespace std;

class BigInt
{
    string number;   // Stores the number as a string
    bool isNegative; // True if number is negative

    // Remove unnecessary leading zeros from the number string

    void removeLeadingZeros()
    {
        if (number.empty())
        {
            number = "0";
            isNegative = false;
            return;
        }

        size_t firstNonZero = number.find_first_not_of('0');

        if (firstNonZero == string::npos)
        {

            number = "0";
            isNegative = false;
        }
        else
        {

            number = number.substr(firstNonZero);
        }

        if (number == "0")
        {
            isNegative = false;
        }
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt &other) const
    {
        string thisNum = number;
        string otherNum = other.number;

        if (thisNum.length() > otherNum.length())
        {
            return 1;
        }
        else if (thisNum.length() < otherNum.length())
        {
            return -1;
        }

        for (size_t i = 0; i < thisNum.length(); i++)
        {
            if (thisNum[i] > otherNum[i])
            {
                return 1;
            }
            else if (thisNum[i] < otherNum[i])
            {
                return -1;
            }
        }
        return 0;
    }

public:
    // Default constructor - initialize to zero

    BigInt()
    {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value)
    {
        if (value == 0)
        {
            number = "0";
            isNegative = false;
            return;
        }

        isNegative = (value < 0);
        if (isNegative)
        {
            value = -value;
        }

        number = "";
        while (value > 0)
        {
            number = char('0' + (value % 10)) + number;
            value /= 10;
        }
    }

    // Constructor from string representation
    BigInt(const string &str)
    {
        if (str.empty())
        {
            number = "0";
            isNegative = false;
            return;
        }

        size_t start = 0;
        if (str[0] == '-')
        {
            isNegative = true;
            start = 1;
        }
        else if (str[0] == '+')
        {
            isNegative = false;
            start = 1;
        }
        else
        {
            isNegative = false;
        }

        number = str.substr(start);

        removeLeadingZeros();

        if (number == "0")
        {
            isNegative = false;
        }
    }

    // Copy constructor
    BigInt(const BigInt &other)
    {
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt()
    {
        // TODO: Implement if needed
    }

    bool getIsNegative() const { return isNegative; }
    const std::string &getDigits() const { return number; }

    // Assignment operator
    BigInt &operator=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (this != &other)
        {
            number = other.number;
            isNegative = other.isNegative;
        }
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const
    {
        BigInt result = *this;
        // TODO: Implement negation logic
        if (result.number != "0")
        {
            result.isNegative = !result.isNegative;
        }
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const
    {
        BigInt result = *this;
        // TODO: Implement this operator
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt &operator+=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (other.number == "0")
        {
            return *this; // Adding zero does nothing
        }
        if (number == "0")
        {
            *this = other; // If we're zero, become the other number
            return *this;
        }
        if (isNegative == other.isNegative)
        {
            string result;
            int carry = 0;
            int i = number.length() - 1;
            int j = other.number.length() - 1;

            while (i >= 0 || j >= 0)
            {
                int digit1 = (i >= 0) ? number[i--] - '0' : 0;
                int digit2 = (j >= 0) ? other.number[j--] - '0' : 0;
                int sum = digit1 + digit2 + carry;
                carry = sum / 10;
                result = char('0' + (sum % 10)) + result;
            }
            number = result;
        }
        else
        {
            BigInt temp = other;
            temp.isNegative = !temp.isNegative;
            *this -= temp;
        }
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt &operator-=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (other.number == "0")
        {
            return *this;
        }

        BigInt negativeOther = other;
        negativeOther.isNegative = !negativeOther.isNegative;

        *this += negativeOther;

        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt &operator*=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (number == "0" || other.number == "0")
        {
            number = "0";
            isNegative = false;
            return *this;
        }
        if (other.number == "1")
        {

            isNegative = (isNegative != other.isNegative);
            return *this;
        }

        BigInt original = *this;
        BigInt counter = other;
        counter.isNegative = false;

        number = "0";
        isNegative = false;

        while (counter.number != "0")
        {
            *this += original;
            counter -= BigInt(1);
        }

        isNegative = (original.isNegative != other.isNegative);

        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt &operator/=(const BigInt &other)
    {
        // TODO: Implement this operator
        if (other.number == "0")
        {
            throw runtime_error("Division by zero!");
        }

        if (number == "0")
        {
            return *this;
        }
        if (other.number == "1")
        {
            isNegative = (isNegative != other.isNegative);
            return *this;
        }

        BigInt dividend = *this;
        BigInt divisor = other;
        BigInt result(0);
        BigInt remainder = dividend;

        dividend.isNegative = false;
        divisor.isNegative = false;
        remainder.isNegative = false;

        while (remainder.compareMagnitude(divisor) >= 0)
        {
            remainder -= divisor;
            result += BigInt(1);
        }

        result.isNegative = (isNegative != other.isNegative);
        *this = result;

        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt &operator%=(const BigInt &other)
    {
        // TODO: Implement this operator
        // a ponter to the first digit;
        if (other.number == "0")
        {
            cout << "Error : you can't devide by 0";
            return *this;
        }
        // just the start.
        BigInt divident = *this;
        BigInt divisor = other;

        // its easier to compute the modulo while the numnbers are postive
        divident.isNegative = false;
        divisor.isNegative = false;

        if (divident.compareMagnitude(divisor) == -1)
        {
            return *this;
        }

        BigInt reminder("0");
        int i = 0;

        while (i < divident.number.size())
        {
            reminder.number += divident.number[i];
            reminder.removeLeadingZeros();

            while (reminder.compareMagnitude(divisor) >= 0)
            {
                reminder -= divisor;
            }

            i++;
        }
        this->number = reminder.number;
        // the modulo result sould not be a negative
        this->isNegative = false;
        this->removeLeadingZeros();

        return *this;
    }

    // Pre-increment operator (++x)
    BigInt &operator++()
    {
        // TODO: Implement this operator
        if (this->isNegative == 0)
        {
            string result = "";
            int right = this->number.size() - 1;
            int num2 = 1;
            int sum = (this->number[right] - '0') + num2;
            result += (sum % 10 + '0');
            int carry = sum / 10;

            while (carry > 0)
            {
                right = right - 1;
                int sum = (this->number[right] - '0') + carry;
                result += (sum % 10 + '0');
                carry = sum / 10;
            }

            if (carry == 0)
            {
                for (int i = right - 1; i >= 0; i--)
                {
                    result += this->number[i] + '0';
                }
                reverse(result.begin(), result.end());
                this->number = result;
                return *this;
            }
        }
        // otherwise the number will be negative. and if the num equal -12 and wee added 1 to it the number will be 11
        string result = "";
        int right = number.size() - 1;
        int num2 = 1;
        if ((this->number[right] - '0') >= 1)
        {
            int subsraction = (this->number[right] - '0') - num2;
            result += (subsraction + '0');
            for (int i = right - 1; i >= 0; i--)
            {
                result += this->number[i] + '0';
            }
            reverse(result.begin(), result.end());
            this->number = result;
            return *this;
        }
        while (number[right] == 0)
        {
            right--;
        }
        int temp = (number[right] - '0') - 1;
        result += (temp + '0');
        right++;
        while (right < number.size())
        {
            if (number[right] == 0)
            {
                result += '9';
            }
            right++;
        }
        reverse(result.begin(), result.end());
        this->number = result;
        if (result == "0")
        {
            this->isNegative = 0;
        }
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int)
    {
        // TODO: Implement this operator
        BigInt temp = *this;
        ++*this;
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt &operator--()
    {
        // TODO: Implement this operator
        if (this->isNegative)
        {
            string result = "";
            int right = this->number.size() - 1;
            int num2 = 1;
            int sum = (this->number[right] - '0') + num2;
            result += (sum % 10 + '0');
            int carry = sum / 10;

            while (carry > 0)
            {
                right = right - 1;
                int sum = (this->number[right] - '0') + carry;
                result += (sum % 10 + '0');
                carry = sum / 10;
            }

            if (carry == 0)
            {
                for (int i = right - 1; i >= 0; i--)
                {
                    result += this->number[i] + '0';
                }
                reverse(result.begin(), result.end());
                this->number = result;
                return *this;
            }
        }
        // otherwise the number will be postive. and if the num equal 12 and wee added -1 to it the number will be 11
        string result = "";
        int right = number.size() - 1;
        int num2 = 1;
        if ((this->number[right] - '0') >= 1)
        {
            int subsraction = (this->number[right] - '0') - num2;
            result += (subsraction + '0');
            for (int i = right - 1; i >= 0; i--)
            {
                result += this->number[i] + '0';
            }
            reverse(result.begin(), result.end());
            this->number = result;
            return *this;
        }
        while (number[right] == 0)
        {
            right--;
        }
        int temp = (number[right] - '0') - 1;
        result += (temp + '0');
        right++;
        while (right < number.size())
        {
            if (number[right] == 0)
            {
                result += '9';
            }
            right++;
        }
        reverse(result.begin(), result.end());
        this->number = result;
        if (result == "0")
        {
            this->isNegative = 0;
        }
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int)
    {
        // TODO: Implement this operator
        BigInt temp = *this;
        --*this;
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const
    {
        // TODO: Implement this function
        if (isNegative && number != "0")
        {
            return '-' + number;
        }
        return number;
    }

    // Output stream operator (for printing)
    friend ostream &operator<<(ostream &os, const BigInt &num)
    {
        // TODO: Implement this operator
        os << num.toString();
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream &operator>>(istream &is, BigInt &num)
    {
        // TODO: Implement this operator
        string input;
        is >> input;
        num = BigInt(input);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<(const BigInt &lhs, const BigInt &rhs);
    friend bool operator<=(const BigInt &lhs, const BigInt &rhs);
    friend bool operator>(const BigInt &lhs, const BigInt &rhs);
    friend bool operator>=(const BigInt &lhs, const BigInt &rhs);
};

// Binary addition operator (x + y)
// 1hs = 123 , rhs = 456

BigInt operator+(BigInt lhs, const BigInt &rhs)
{
    BigInt result;
    // TODO: Implement this operator
    BigInt result = lhs;
    result += rhs;
    return result;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt &rhs)
{
    BigInt result;
    // TODO: Implement this operator
    BigInt result = lhs;
    result -= rhs;
    return result;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt &rhs)
{
    BigInt result;
    // TODO: Implement this operator
    BigInt result = lhs;
    result *= rhs;

    return result;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt &rhs)
{
    BigInt result;
    // TODO: Implement this operator
    BigInt result = lhs;
    result /= rhs;
    return result;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt &rhs)
{
    BigInt result;
    // TODO: Implement this operator
    BigInt result = lhs;
    result %= rhs;
    return result;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    return (lhs.isNegative == rhs.isNegative && lhs.number == rhs.number);
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt &lhs, const BigInt &rhs)
{
    //    1
    // First, handle the simple case: compare the signs.

    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return true;
    }
    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return false;
    }
    // 2. Both numbers have the SAME sign (both positive or both negative)
    const std ::string &lhsdigit = lhs.getDigits();
    const std ::string &rhsdigit = rhs.getDigits();
    // comare number of digits
    if (lhsdigit.size() != rhsdigit.size())
    {
        return !lhs.getIsNegative(); // if both positive return true else false
    }
    if (lhsdigit.size() > rhsdigit.size())
    {
        return lhs.getIsNegative(); // if both negative return true else false
    }
    // 3. Same number of digits: compare digit by digit
    // from left to right
    for (size_t i = 0; i < lhsdigit.size(); ++i)
    {
        if (lhsdigit[i] < rhsdigit[i])
        {
            return !lhs.getIsNegative(); // if both positive return true else false{
        }
        if (lhsdigit[i] > rhsdigit[i])
        {
            return lhs.getIsNegative(); // if both negative return true else false
        }
    }
    return false;
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    // 1. Handle different signs
    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return true; // -VE > +VE
    }
    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return false; // +VE > -VE
    }
    // 2. Both have same sign - compare
    int cmp = lhs.compareMagnitude(rhs);
    if (lhs.getIsNegative())
    {
        return cmp > 0; // both negative
    }
    else
    {
        return cmp < 0; // both positive
    }
    return false;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator
    // 1. Handle different signs
    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return true; // -VE <= +VE
    }

    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return false; // +VE => -VE
    }
    int cmp = lhs.compareMagnitude(rhs);
    if (cmp == 0)
        return true; // equal
    if (lhs.getIsNegative())
    {
        return cmp > 0; // both negative
    }
    else
    {
        return cmp < 0; // both positive
    }

    // 2. Both have same sign - compare
    return false;
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt &lhs, const BigInt &rhs)
{
    // TODO: Implement this operator'
    //    4
    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return false; // -VE < +VE
    }
    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return true; // +VE > -VE
    }
    int cmop = lhs.compareMagnitude(rhs);
    if (lhs.getIsNegative())
    {
        return cmop < 0; // both negative
    }
    else
    {
        return cmop > 0; // both positive
    }
    return false;
    /*
    return rhs < lhs;
    */
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt &lhs, const BigInt &rhs)
{
    if (lhs.getIsNegative() && !rhs.getIsNegative())
    {
        return false; // -VE < +VE
    }
    if (!lhs.getIsNegative() && rhs.getIsNegative())
    {
        return true; // +VE > -VE
    }
    int cmp = lhs.compareMagnitude(rhs);
    if (cmp == 0)
        return true; // equal
    if (lhs.getIsNegative())
    {
        return cmp < 0; // both negative
    }
    else
    {
        return cmp > 0; // both positive
    }
    return false;
}

int main()
{
    cout << "=== BigInt Class Test Program ===" << endl
         << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl
         << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
