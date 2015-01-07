#include "flowgraph/entities/entity.h"


// Overloaded Constructor
CEntity::CEntity(char* name)
{
	// copy entity name
	m_Attributes["name"].type = ATYPE_STRING;
	sprintf_s(m_Attributes["name"].data.str_val, 4096, name);

	// store its position
	//m_Attributes["position"].type = ATYPE_MATH_VECTOR;
	//m_Attributes["position"].data = position;
}

// Destructor
CEntity::~CEntity()
{
	// nothing to destruct
}

		
// Public get methods to access private buffers
void CEntity::GetData(char* key, bool* out) {
	*out = m_Attributes[key].data.bool_val;
}
void CEntity::GetData(char* key, float* out) {
	*out = m_Attributes[key].data.float_val;
}
void CEntity::GetData(char* key, double* out) {
	*out = m_Attributes[key].data.double_val;
}
void CEntity::GetData(char* key, long double* out) {
	*out = m_Attributes[key].data.long_double_val;
}
void CEntity::GetData(char* key, char* out) {
	out = m_Attributes[key].data.str_val;
}
void CEntity::GetData(char* key, wchar_t* out) {
	out = m_Attributes[key].data.wchart_str_val;
}
void CEntity::GetData(char* key, signed short* out) {
	*out = m_Attributes[key].data.signed_short_val;
}
void CEntity::GetData(char* key, unsigned short* out) {
	*out = m_Attributes[key].data.unsigned_short_val;
}
void CEntity::GetData(char* key, signed int* out) {
	*out = m_Attributes[key].data.signed_int_val;
}
void CEntity::GetData(char* key, unsigned int* out) {
	*out = m_Attributes[key].data.unsigned_int_val;
}
void CEntity::GetData(char* key, signed long* out) {
	*out = m_Attributes[key].data.signed_long_val;
}
void CEntity::GetData(char* key, unsigned long* out) {
	*out = m_Attributes[key].data.unsigned_long_val;
}
void CEntity::GetData(char* key, signed long long* out) {
	*out = m_Attributes[key].data.signed_long_long_val;
}
void CEntity::GetData(char* key, unsigned long long* out) {
	*out = m_Attributes[key].data.unsigned_long_long_val;
}


// Set Data
void CEntity::SetData(char* key, bool in) {
	m_Attributes[key].type = ATYPE_BOOL;
	m_Attributes[key].data.bool_val = in;
}
void CEntity::SetData(char* key, float in) {
	m_Attributes[key].type = ATYPE_FLOAT;
	m_Attributes[key].data.float_val = in;
}
void CEntity::SetData(char* key, double in) {
	m_Attributes[key].type = ATYPE_DOUBLE;
	m_Attributes[key].data.double_val= in;
}
void CEntity::SetData(char* key, long double in) {
	m_Attributes[key].type = ATYPE_LONG_DOUBLE;
	m_Attributes[key].data.long_double_val = in;
}
void CEntity::SetData(char* key, char* in) {
	m_Attributes[key].type = ATYPE_STRING;
	strcpy_s(m_Attributes[key].data.str_val, 4096, in);
}
void CEntity::SetData(char* key, wchar_t* in) {
	m_Attributes[key].type = ATYPE_WSTRING;
	wcscpy_s(m_Attributes[key].data.wchart_str_val, 4096, in);
}
void CEntity::SetData(char* key, signed short in) {
	m_Attributes[key].type = ATYPE_SIGNED_SHORT;
	m_Attributes[key].data.signed_short_val = in;
}
void CEntity::SetData(char* key, unsigned short in) {
	m_Attributes[key].type = ATYPE_UNSIGNED_SHORT;
	m_Attributes[key].data.unsigned_short_val = in;
}
void CEntity::SetData(char* key, signed int in) {
	m_Attributes[key].type = ATYPE_SIGNED_INT;
	m_Attributes[key].data.signed_int_val = in;
}
void CEntity::SetData(char* key, unsigned int in) {
	m_Attributes[key].type = ATYPE_UNSIGNED_INT;
	m_Attributes[key].data.unsigned_int_val = in;
}
void CEntity::SetData(char* key, signed long in) {
	m_Attributes[key].type = ATYPE_SIGNED_LONG;
	m_Attributes[key].data.signed_long_val = in;
}
void CEntity::SetData(char* key, unsigned long in) {
	m_Attributes[key].type = ATYPE_UNSIGNED_LONG;
	m_Attributes[key].data.unsigned_long_val = in;
}
void CEntity::SetData(char* key, signed long long in) {
	m_Attributes[key].type = ATYPE_SIGNED_LONG_LONG;
	m_Attributes[key].data.signed_long_long_val= in;
}
void CEntity::SetData(char* key, unsigned long long in) {
	m_Attributes[key].type = ATYPE_UNSIGNED_LONG_LONG;
	m_Attributes[key].data.unsigned_long_long_val = in;
}
