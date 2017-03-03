/*
Author: Abhishek Chander
eMail: abhishekc20(@)hotmail(dot)com
*/

#include<iostream>
#include<string>
#include<vector>
#include<math.h>
#include<stdlib.h>

using namespace std;

struct InvalidCharacterException{
} ice;

struct InvalidSubtractionException{
} ise;

struct DivideByZeroException{
} dbze;

struct InvalidBigIntAssignmentException{
} ibiae;

struct NegativeExponentException{
} nee;

class BigInt {
	vector<char> bigVec;
public:
	BigInt(){}
	BigInt(string);
	BigInt(vector<char>);
	BigInt(int); // Assigns the int to a new BigInt
	void operator=(int); // Converts the int to the BigInt
	long int getSize(); // Returns the number of digits in the decimal representation of the BigInt
	vector<char> getVector(); // Returns the vector<char>
	BigInt operator+(BigInt); // Addition
	BigInt operator+(int);
	void operator+=(BigInt); 
	void operator+=(int);
	BigInt operator-(BigInt); // Subtraction. Throws Exception if second is bigger than this.
	BigInt operator-(int);
	void operator-=(BigInt);
	void operator-=(int);
	BigInt slowMult(BigInt); // Slower than operator*. Can be used to see difference in speed
	BigInt operator*(BigInt); // Very fast multiplication!
	BigInt operator*(int);
	void operator*=(BigInt);
	void operator*=(int);
	BigInt pow(BigInt); // Returns this^exp
	BigInt pow(int);
	BigInt* slowDiv(BigInt); // Returns A/B, and its remainder, but is slower than operator/. It is however, used by operator/
	// Also, throws Exception if divisor is 0
	BigInt divHelper(BigInt, int); // Helper, to provide functions for both operator/ and operator%
	BigInt operator/(BigInt); // Very fast division!
	BigInt operator/(int);
	void operator/=(BigInt);
	void operator/=(int);
	BigInt operator%(BigInt); // Returns the modulus
	BigInt operator%(int);
	void operator%=(BigInt);
	void operator%=(int);
	int compare(BigInt); // Returns 1 if this is bigger than second, -1 if this is smaller, 0 if equal
	int compare(int);
	bool operator==(BigInt); // Tests for equality
	bool operator==(int);
	void display(); // Outputs BigInt to console
};

vector<char> myFuncReduce0(vector<char>); // removes any leading zeros
vector<char> myFuncReduce(vector<char>); // used by operator*

int main(){	
	string ms="";
	string os="";
	
	cin>>ms;
	cin>>os;
	
	BigInt A(ms);
	BigInt B(os);
	
	try{
		BigInt C = BigInt(); 
		
        // Mini test case: 1143987589345/13584375 = 84213.4871383
//		C = A*B;
		C = A/B;
        
		cout<<"C:";
		C.display();
		
	} catch (InvalidSubtractionException e){
		cout<<"Cannot subtract from smaller number." <<endl;
	} catch (DivideByZeroException e){
		cout<<"Cannot divide by zero." <<endl;
	}

	return 0;
}

BigInt::BigInt(string s){
	int sSize = s.size();
	bigVec.assign(sSize, 0);
	for(int i=sSize-1, j = 0; i>=0; i--, j++){
		if (s[i] >= '0' && s[i] <= '9'){
			bigVec[j] = s[i];
		} else {
			throw ice;
		}   
	}
	bigVec = myFuncReduce0(bigVec);
}

BigInt::BigInt(vector<char> vc){
	this->bigVec = myFuncReduce0(vc);
}

// Assigns the int to a new BigInt
BigInt::BigInt(int decInt){
	*this = decInt;
}

// Converts the int to the BigInt
void BigInt::operator=(int decInt){
	if (decInt < 0) {
		throw ibiae;
	}
	if (decInt == 0){
		vector<char> zero(1, '0');
		this->bigVec = zero;
		return;
	}
	int size = 1 + log10(decInt);
	vector<char> newV(size, '0');
	this->bigVec = newV;
	for(int i = 0; decInt > 0; i++, decInt /= 10){
		bigVec[i] += decInt % 10;
	}
} 

// Returns the number of digits in the decimal representation of the BigInt
long int BigInt::getSize(){
	return (bigVec.size());
}

// Returns the vector<char>
vector<char> BigInt::getVector(){
	return (bigVec);
}

// Addition
BigInt BigInt::operator+(BigInt second){
	long int firstSize = this->getSize();
	long int secondSize = second.getSize();
	long minL, maxL;
	minL = min(firstSize, secondSize);
	maxL = max(firstSize, secondSize);
	
	vector<char> firstVec, secondVec;
	if(firstSize < secondSize){
		firstVec = second.getVector();
		secondVec = this->getVector();
	} else {
		firstVec = this->getVector();
		secondVec = second.getVector();
	}
	
	for(long int i=0; i<minL; i++){
		firstVec[i] += secondVec[i] - '0';		
	}
	
	firstVec.reserve(maxL+1);
	firstVec.resize(maxL+1);
	firstVec[maxL] = '0';
	for(long int i=0; i<maxL; i++){
		if (firstVec[i] > '9' && firstVec[i] < ('9' + 11)) {
			firstVec[i+1] += 1;
			switch (firstVec[i] - '9'){
			case 1:firstVec[i]='0';break;
			case 2:firstVec[i]='1';break;
			case 3:firstVec[i]='2';break;
			case 4:firstVec[i]='3';break;
			case 5:firstVec[i]='4';break;
			case 6:firstVec[i]='5';break;
			case 7:firstVec[i]='6';break;
			case 8:firstVec[i]='7';break;
			case 9:firstVec[i]='8';break;
			case 10:firstVec[i]='9';
			}
		}
	}
	if(firstVec[maxL] == '0') {
		firstVec.reserve(maxL);
		firstVec.resize(maxL);
	}
	
	BigInt ans(firstVec);
	return ans;
}

BigInt BigInt::operator+(int decInt){
	BigInt B = decInt;
	B = (*this) + B;
	return B;
}

void BigInt::operator+=(BigInt second){
	*this = *this + second;
}

void BigInt::operator+=(int decInt){
	BigInt B = decInt;
	*this += B;
}

// Subtraction. Throws Exception if second is bigger than this.
BigInt BigInt::operator-(BigInt second){
	long int firstSize = this->getSize();
	long int secondSize = second.getSize();
	int w=this->compare(second);
	
	if (w < 0) {
		throw ise;
	} else if(w == 0){
		vector<char> temp(1,'0');
		BigInt ans(temp);
		return ans;
	}
	
	vector<char> firstVec = this->getVector();
	vector<char> secondVec = second.getVector();
	
	for(int i=0; i<secondSize; i++){
		firstVec[i] += (int)('0' - (int)secondVec[i]);
	}
	for(int i=0; i<firstSize-1; i++){
		if (firstVec[i] >= ('0' - 10) && firstVec[i] <= ('0' - 1)){
			firstVec[i+1] -= 1;
			// instead of firstVec[i] = '0' - 10, etc
			switch ('0' - firstVec[i]){
			case 10:firstVec[i]+=10; break;
			case 9:firstVec[i]+=10; break;
			case 8:firstVec[i]+=10; break;
			case 7:firstVec[i]+=10; break;
			case 6:firstVec[i]+=10; break;
			case 5:firstVec[i]+=10; break;
			case 4:firstVec[i]+=10; break;
			case 3:firstVec[i]+=10; break;
			case 2:firstVec[i]+=10; break;
			case 1:firstVec[i]+=10;
			}
		}
	}
	
	while(firstVec[firstSize-1]<'1' && firstSize>1){
		firstSize--;
		firstVec.reserve(firstSize);
		firstVec.resize(firstSize);
	}
	
	BigInt ans(firstVec);
	return ans;
}

BigInt BigInt::operator-(int decInt){
	BigInt B = decInt;
	B = *this - B;
	return B;
}

void BigInt::operator-=(BigInt second){
	*this = *this - second;
}

void BigInt::operator-=(int decInt){
	BigInt B = decInt;
	*this -= B;
}

// Slower than operator*. Can be used to see difference in speed
BigInt BigInt::slowMult(BigInt second){
	long int firstSize = this->getSize();
	long int secondSize = second.getSize();
	
	BigInt A, B;
	if(firstSize < secondSize){
		A = second;
		B = *this;
	} else {
		A = *this;
		B = second;
	}
	
	long int maxL = max(firstSize, secondSize);
	long int minL = min(firstSize, secondSize);

	vector<char> oneVec(1, '1');
	BigInt one(oneVec);
	
	vector<char> zeroVec(1, '0');
	BigInt ans(zeroVec);
	
	while((B.compare(one) >= 0)){
		ans = ans + A;
		minL--;
		B = B - one;
	}
	return ans;
}

// Very fast multiplication!
BigInt BigInt::operator*(BigInt second) {
	
	long int firstSize = this->getSize();
	long int secondSize = second.getSize();
	
	BigInt A, B;
	if(firstSize < secondSize){
		A = second;
		B = *this;
	} else {
		A = *this;
		B = second;
	}
	
	vector<char> vecA = A.getVector();
	vector<char> vecB = B.getVector();
	
	long int aSize = vecA.size();
	long int bSize = vecB.size();
	
	// To hold calculations temporarily
	vector<char> tempVec(vecA);
	long int tempVecSize = aSize;
	BigInt tempBGNT(tempVec);
	
	vector<char> tempVecA(vecA);
	
	A = BigInt("0");
	
	for(long int i=0; i<bSize; i++){
		tempVecSize += i;
		tempVec.reserve(tempVecSize);
		tempVec.resize(tempVecSize);
		
		for(int h=tempVecSize-1; h>0; h--){
			// shift all elements by i
			if(h>=i){
				tempVec[h] = tempVec[h-i];
			} else {
				tempVec[h] = 0;
			}
		}
		
		for(int j = i; j<tempVecSize; j++){
			// Here, the number is stored in a char code, but starting from 0,
			// because ASCII only goes up to 127
			// we retrieve the number later
			tempVec[j] = (tempVec[j] - '0') * (vecB[i] - '0');
		}
		
		tempVec = myFuncReduce(tempVec);
		tempBGNT = BigInt(tempVec);
		A = A + tempBGNT;
		tempVec = tempVecA;
		tempVecSize = tempVec.size();
	}
	return A;
}

BigInt BigInt::operator*(int decInt){
	BigInt B = decInt;
	B = *this * B;
	return B;
}
void BigInt::operator*=(BigInt second){
	*this = *this * second;
}
void BigInt::operator*=(int decInt){
	BigInt B = decInt;
	*this = *this * B;
}

// Returns this^exp
BigInt BigInt::pow(BigInt exp){
	int mag = exp.compare(0);
	switch(mag){
	case -1: throw nee;
	case 0: return (BigInt(1));
	}
	BigInt A = *this;
	if (exp == 1){
		return A;
	}
	BigInt ans;
	if ((exp % 2) == 0) {
		exp /= 2;
		A = A * A;
		ans = A.pow(exp);
	} else {
		exp = (exp - 1) / 2;
		ans = A * ((A * A).pow(exp));
	}
	return ans;
}

// return this^exp
BigInt BigInt::pow(int decInt){
	BigInt exp = decInt;
	return (this->pow(exp));
}

/*
Returns A/B, and its remainder, but is slower than operator/
It is however, used by operator/
Also, throws Exception if divisor is 0
*/
BigInt* BigInt::slowDiv(BigInt second){
	vector<char> oneVec(1,'1');
	BigInt one(oneVec);
	
	vector<char> zeroVec(1,'0');	
	BigInt quotient(zeroVec);
	
	if (second.compare(quotient) == 0){
		throw dbze;
	}
	
	long int firstSize = this->getSize();
	long int secondSize = second.getSize();
	
	BigInt A = *this;
	BigInt B = second;
	
	try{
		while(true){
			// ordering is crucial
			A = A - B;
			quotient = quotient + one;
		}
	} catch (InvalidSubtractionException e) {
		/* Stored on heap, because we return a pointer to it
			This function is only to be used on operator/
			which will also deallocate this from heap
		*/
		BigInt *ans = new BigInt[2];
		ans[0] = quotient;
		ans[1] = A; // remainder
		return ans;
	}
}

// Very fast division!
BigInt BigInt::divHelper(BigInt divisor, int arg){
	long int comparison = this->compare(divisor);
	BigInt zero("0");
	
	if (divisor.compare(zero) == 0){
		throw dbze;
	}
	
	if(comparison <0){
		return zero;
	} else if (comparison == 0) {
		switch(arg){
		case 0: return (BigInt(1));
		case 1: return (zero);
		}
	}
	
	if(this->compare(zero) == 0){
		return zero;
	}
	
	vector<char> vecDividend = this->getVector();
	long int dividendCounter = vecDividend.size();
	vector<char> tempVecDividend(1, '0'); //dividend
	long int tempVecDividendSize = 0;
	tempVecDividend[0] = vecDividend[vecDividend.size()-1];
	BigInt tempDividend(tempVecDividend);
	
	vector<char> tempVec(1, '0'); //temporary vector
	long int tempVecSize = 1;
	BigInt tempBGNT(tempVec);
	
	vector<char> vecQuot(1, '0'); //quotient
	long int vecQuotSize = 0;
	
	BigInt* tempBGNTarray;
	
	while(dividendCounter > 0){
		comparison = tempDividend.compare(divisor);
		
		vecQuotSize++;
		vecQuot.reserve(vecQuotSize);
		vecQuot.resize(vecQuotSize);
		
		tempVecDividendSize++;
		tempVecDividend.reserve(tempVecDividendSize);
		tempVecDividend.resize(tempVecDividendSize);

		for(int i = tempVecDividendSize-1; i>0; i--){
			tempVecDividend[i] = tempVecDividend[i-1];
		}
	
		tempVecDividend[0] = vecDividend[dividendCounter-1];
		tempDividend = BigInt(tempVecDividend);
		tempVecDividendSize = tempVecDividend.size();
		
		for(int i = vecQuotSize-1; i>0; i--){
			// shift all elements by 1
			vecQuot[i] = vecQuot[i-1];
		}
		
		tempBGNTarray = tempDividend.slowDiv(divisor);
		tempBGNT = tempBGNTarray[0];
		tempDividend = tempBGNTarray[1]; // remainder of that calculation
		delete []tempBGNTarray;// crucial to avoid memory leaks
		
		tempVecDividend = tempDividend.getVector();
		tempDividend = BigInt(tempVecDividend);
		tempVec = tempBGNT.getVector();
		vecQuot[0] = tempVec[0];
		
		dividendCounter--;
	}

	BigInt quotient(vecQuot);
	switch(arg){
	case 0: return quotient;
	case 1: return tempDividend; // remainder
	}
    return zero;
}

BigInt BigInt::operator/(BigInt divisor){
	BigInt ans = this->divHelper(divisor, 0);
	return ans;
}

BigInt BigInt::operator/(int decInt){
	BigInt B = decInt;
	B = *this / B;
	return B;
}

void BigInt::operator/=(BigInt second){
	*this = *this / second;
}

void BigInt::operator/=(int decInt){
	BigInt B = decInt;
	*this = *this / B;
}

// Returns the modulus
BigInt BigInt::operator%(BigInt second){
	BigInt ans = this->divHelper(second, 1);
	return ans;
}

BigInt BigInt::operator%(int decInt){
	BigInt B = decInt;
	B = *this % B;
	return B;
}

void BigInt::operator%=(BigInt second){
	*this = *this % second;
}

void BigInt::operator%=(int decInt){
	BigInt B = decInt;
	*this = *this % B;
}

// Returns 1 if this is bigger than second, -1 if this is smaller, 0 if equal
int BigInt::compare(BigInt second){
	long int aSize = this->getSize();
	long int bSize = second.getSize();
	
	if(!(aSize == bSize)){
		return (aSize - bSize);
	}
	
	vector<char> aVec = this->getVector();
	vector<char> bVec = second.getVector();
	int ans;
	for (int i = aSize-1; i>=0; i--){
		if (!(aVec[i] == bVec[i])) {
			ans = aVec[i] - bVec[i];
			return (ans);
		}
	}
	return 0;
}

int BigInt::compare(int decInt){
	BigInt B = decInt;
	return (this->compare(B));
}

// Tests for equality
bool BigInt::operator==(BigInt second){
	return (this->compare(second) == 0);
}

bool BigInt::operator==(int decInt){
	BigInt B = decInt;
	return (*this == B);
}

// Outputs BigInt to console
void BigInt::display(){
	for(long int i=bigVec.size()-1; i>=0; i--){
		cout<<bigVec[i];
	}
	cout<<endl;
}

/* NOTE: only to be used in operator*
	Because it works on vector<char> where
	the ASCII character codes represent the
	numbers
*/
vector<char> myFuncReduce(vector<char> V){
	long int vSize = V.size();
	V.reserve(vSize+1);
	V.resize(vSize+1);
	V[vSize] = 0;
	
	for(long int i=0; i<=vSize; i++){
        int ith = (int)V[i];
        V[i] = ith % 10;
        V[i+1] = i / 10;
        // TODO: check to see if there is a potential overflow bug here.
    }
	if(V[vSize] == 0 || V[vSize] == '0') {
		V.reserve(vSize);
		V.resize(vSize);
	}
	return V;
}

// Removes any leading zeros
vector<char> myFuncReduce0(vector<char> V){
	long int vSize = V.size();
	while((V[vSize-1] == '0' || V[vSize-1] == 0) && (vSize>1)){
		vSize--;
	}
	if (vSize < V.size()){
		V.reserve(vSize);
		V.resize(vSize);
	}
	return V;
}
