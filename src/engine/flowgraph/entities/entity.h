/*
* New entity system for Inexor
* Based on Hanack's model
*/

// Include guard prevents this file from being included twice
#ifndef INEXOR_ENTITY_ENGINE_HEADER
#define INEXOR_ENTITY_ENGINE_HEADER

// Include standard libraries
#include <string>
#include <map>
#include <vector>
#include <list>
//#include <stack>
//#include <deque>
//#include <iterator>


// Enumeration for data type description
// We should discuss data types!
enum ATTRIBUTE_DATA_TYPE 
{
	ATYPE_BOOL = 0,
	ATYPE_VOID, // not used yet
	ATYPE_FLOAT,
	ATYPE_DOUBLE,
	ATYPE_LONG_DOUBLE,
	//ATYPE_CHAR, // we don't need those
	//ATYPE_WCHAR,
	ATYPE_WSTRING,
	ATYPE_STRING,
	ATYPE_SIGNED_SHORT,
	ATYPE_UNSIGNED_SHORT,
	ATYPE_SIGNED_INT,
	ATYPE_UNSIGNED_INT,
	ATYPE_SIGNED_LONG,
	ATYPE_UNSIGNED_LONG,
	ATYPE_SIGNED_LONG_LONG, // Please note unsigned long long is i64
	ATYPE_UNSIGNED_LONG_LONG,

	ATYPE_MATH_VECTOR // can we even use this one? 
};


// Union to store data
union ATTRIBUTE_DATA
{
	bool bool_val;
	//void void_val; // not used yet
	float float_val;
	double double_val;
	long double long_double_val;

	// it does not make sense to support these...
	//char char_val;
	//wchar_t wchart_val;
	wchar_t wchart_str_val[4096]; // probably biggest
	char str_val[4096];
	signed short signed_short_val;
	unsigned short unsigned_short_val;
	signed int signed_int_val;
	unsigned int unsigned_int_val;
	signed long signed_long_val;
	unsigned long unsigned_long_val;
	signed long long signed_long_long_val;
	unsigned long long unsigned_long_long_val;

	// composed data types
	//vec float_vector;
};


// Structure to describe attributes
struct S_ATTRIBUTE 
{
	ATTRIBUTE_DATA_TYPE type;
	ATTRIBUTE_DATA data;
};


// Entity class
// All data is being handled by attributes
// All relations are being handled by m_Childs and m_Parents
class CEntity
{
	public:

		// Overloaded Constructor
		CEntity(char* name);

		// Destructor
		~CEntity();

		
		// Public get methods to access private buffers
		void GetData(char* key, bool* out);
		void GetData(char* key, float* out); // do we really have to make it all const?
		void GetData(char* key, double* out);
		void GetData(char* key, long double* out);
		void GetData(char* key, char* out); // string
		void GetData(char* key, wchar_t* out); // wide string
		void GetData(char* key, signed short* out);
		void GetData(char* key, unsigned short* out);
		void GetData(char* key, signed int* out);
		void GetData(char* key, unsigned int* out);
		void GetData(char* key, signed long* out);
		void GetData(char* key, unsigned long* out);
		void GetData(char* key, signed long long* out);
		void GetData(char* key, unsigned long long* out);

		// Set Data
		void SetData(char* key, bool in);
		void SetData(char* key, float in);
		void SetData(char* key, double in);
		void SetData(char* key, long double in);
		void SetData(char* key, char* in); // string
		void SetData(char* key, wchar_t* in); // wide string
		void SetData(char* key, signed short in);
		void SetData(char* key, unsigned short in);
		void SetData(char* key, signed int in);
		void SetData(char* key, unsigned int in);
		void SetData(char* key, signed long in);
		void SetData(char* key, unsigned long in);
		void SetData(char* key, signed long long in);
		void SetData(char* key, unsigned long long in);

	private:

		// All childs will be stored here
		std::vector<CEntity*> m_Childs;

		// All parents will be stored here
		std::vector<CEntity*> m_Parents;

		// Attribute hashset
		std::map<char*, S_ATTRIBUTE> m_Attributes;
};

#endif // INEXOR_ENTITY_ENGINE_HEADER not defined?