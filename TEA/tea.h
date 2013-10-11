#ifndef TEA_H
#define TEA_H
 
typedef unsigned long ulong;

class TEA {
public:
	TEA(const byte *key, int round = 16);
	TEA(const TEA &rhs);
	TEA& operator=(const TEA &rhs);
	~TEA(); 
	void encrypt(const ulong *in, ulong *out);
	void decrypt(const ulong *in, ulong *out);
private:
	int _round; //iteration round to encrypt or decrypt
	byte *_key;//[16]; //encrypt or decrypt key
private: 
	inline ulong ntoh(ulong netlong) 
	{
		return netlong; 
	}
	inline ulong hton(ulong hostlong)
	{
		return  hostlong; 
	}

};

#endif/*TEA_H*/