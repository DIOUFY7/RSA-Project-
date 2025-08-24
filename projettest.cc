//----------------------------------------------------------------------------------------
//
// NOM: DIOUF       Prénom: Mamadou             Groupe: A3
//
//-----------------------------------------------------------------------------------------
// -*- compile-command: "g++ projettest.cc -lgmp" -*-
#include <iostream>
using namespace std;
#include <vector>
#include <gmpxx.h>
typedef mpz_class Integer;
Integer puissance1(Integer b, Integer exp)
{
  Integer c=1;
    for (Integer j=0;j<exp;++j){
      c=(c*b);}
    return c;
    }
Integer pmod( const Integer& a, const Integer& m )
{
  if ( m == 0 ) return a;
  Integer r= a%m;
  if ( r<0 ) r+= abs(m);
  return r;
}
Integer pgcd(Integer a, Integer  b)
{
    while (b != 0)
    {
        const Integer t = b;
        b = a%b;
        a=t;
    }
    return a;
}
Integer inverse_mod ( Integer a, const Integer& mod)
{if (pgcd(a,mod)==1){
    for (Integer u=1;u<mod;u++){
      if ((a*u)%mod==1){return u;}
    }
  }
  else {return 0;
  }
}
//-----------------------CLE PRIVE AVEC p et q-----------------------------
//Pour avoir la clé privé d de n=pq(mod) (avec p et q des entiers premiers)
// et c premier avec phi(n)=(p-1)(q-1) nous avons d=inverse_mod(c, phi(n))
//--------------------------------------------------------------------------
Integer puissance( Integer base, Integer exp, const Integer& mod )
{ if (exp>=0 and mod>=1) {
    Integer c=1;
    for (Integer j=0;j<exp;++j){
     c=(c*base)%mod;}

    return c;}
  else
    return base;

  }
  //-----------------------------------CONVERT---------------------------------
  //La fonction convert donne le developpement en base de bdu vecteur chiffres
  //---------------------------------------------------------------------------
Integer convert( const vector<Integer>& chiffres, const Integer& b )
{int     n=chiffres.size();
  Integer entier=0;
  int i=0;
  while (i<n){
    Integer r=chiffres[i];
    entier=entier+r*(puissance1(b,i));
    i=i+1;}
  return entier;
}
//-------------------------------------CONVERT1------------------------------
//convert1 donne le vecteur de la factorisation en base de b de l'entier n
//--------------------------------------------------------------------------
vector<Integer> convert1( Integer n, const Integer& b )
{vector<Integer> chiffres;
  Integer q;
  Integer r;
  while (n!=0)
    {mpz_fdiv_qr(q.get_mpz_t(),r.get_mpz_t(),n.get_mpz_t(),b.get_mpz_t());
      chiffres.push_back(r);
      n=q;
    }
  return chiffres;
}
//------------------------CHAR_TO_INTEGER----------------------------
// Transformer une lettre en un entier
// Les lettres A,...,Z (ou a,...,z) codent les nombres 11,...,36.
//les chiffres 0 à 9 renvoyés sur eux mêmes
// L'underscore '_' (ou toute autre lettre) est envoyé sur 10.
//-------------------------------------------------------------------------
Integer char_to_integer( char c )
{
  if ( c >= 'A' && c <= 'Z' ) return Integer(c-'A'+11);
  if ( c >= 'a' && c <= 'z' ) return Integer(c-'a'+11);
  if (c >='0' && c <= '9') return Integer(c - 48);
  return 10;
}

//-------------------------INTEGER_TO_CHAR------------------------------------
// Transformer un entier en une lettre
//Les nombres 0 à 9 codent 0 à 9
// Les nombres 11,...,36 codent les lettres A,...,Z
// Toute autre valeur est envoyée sur underscore '_'
//-------------------------------------------------------------------------
char integer_to_char( Integer n )
{
  if ( n < 0 || n > 36 || n==10) return '_';
  if (n >= 0 && n < 10) return char( n.get_ui() + 48);
  return char( n.get_ui() - 11 + 'A' );
}
//---------------------------CONVT--------------------------------------------
//La fonction convt convertit un texte en un entier (integer) grace au vecteur
//chiffres, dont les coordonnées sont le char_to_integer de chaque caractère de
//texte par ordre croissance , appliqué à la fonction convert avec b=37
//------------------------------------------------------------------------------
Integer convt(const string& texte)
{
  vector<Integer> chiffres;
  Integer n=texte.size();
  for  (size_t i=0; i<n;++i)
    {Integer c=char_to_integer(texte[i]);
      chiffres.push_back(c);
    }
  Integer b=37;
  return convert(chiffres, b);
}
//-----------------------------------------CONVT1----------------------------------
//convt1 convertit un nombre en texte en le développant en base de 37, grace à la
//convert1, puis convertit chaque coordonnées (integer) en char par integer_to_char
//---------------------------------------------------------------------------------
string convt1(Integer nombre)
{Integer b=37;
  vector<Integer> v=convert1(nombre, b);
    int n=v.size();
    char text[n];


    for (int i=0;i<n;++i)
    {char d=integer_to_char(v[i]);
      text[i]=d;

      }
  return text;
}
//---------------------------------CRYPTAGE_RSA--------------------------------------------
//La fonction cryptageRSA reçoit la clé public (n=mod, c=exp) et le texte
//et transforme le texte à caractères dans {A,.....,Z,0,...,9,_}, le convertit en
// nombre m (integer) qui sera développé en un vecteur mk de base mod dont les coordonnées
// seront cryptées par la fonction puissance(modulaire) mk1[i]=mk[i]^c mod n afin de
//convertir en Integer m1 puis en string par convt1 et de retourner le crypter texte1
//-----------------------------------------------------------------------------------------
string cryptageRSA( const Integer& mod, const Integer& exp, string& texte )
{Integer mm = convt(texte);
  string text1=convt1(mm);
  Integer m=convt(text1);
  vector<Integer> mk=convert1(m,mod);
  int n=mk.size();
  vector<Integer> mk1(n);
  for (int i=0;i<n;++i){
    mk1[i]=puissance( mk[i], exp, mod );
  }
  Integer m1=convert(mk1,mod);
  string texte1=convt1(m1);
  return texte1;
}
//-------------------------------------------------------------------------------------
//Pour le décryptage, il suffit juste de remplacer c=(exp) par la clé publique d
//car nous cd=1 mod phi(n) et mk1[i]=mk[i]^c mod n ==> mk1[i]^d=mk[i]^(cd)=mk[i] mod n
//-------------------------------------------------------------------------------------
int main()
{ cout<<"entrez votre texte svp"<<endl;
  string text;
  cin>>text;
  cout<<"entrez votre clé public n(mod) puis c(exp) svp"<<endl;
  string mm;
  string ll;
  cin>>mm>>ll;
  Integer n=Integer(mm);
  Integer c=Integer(ll);
  string tet=cryptageRSA(n, c, text);
  cout<<"le texte (dé)crypté est   "<<tet<<endl;
  //Pour décrypter
  cout<<"entrez votre code privé"<<endl;
  string co;
  cin>>co;
  Integer d=Integer(co);
  string tet1=cryptageRSA(n, d, tet);
  cout<<"le texte décrypté est"<<tet1<<endl;
}
//exemple de clé n=187 c=23 d=17
//--------------------------------CONCLUSION-----------------------------------------------
//Après avoir fait des tests mon algorithme marche.
// Comme vous le savez je me suis inspiré
//du projet Cryptographie RSA du poly de cours. Cependant, il y a des différences entre les
// fonctions mais aussi niveau des résultats. Dans le poly, ils utilisent une base de 27 ce
// qui nous permet d'avoir seulement les lettres alors j'ai pensé une base de 37 qui permet
// d'avoir les lettres et les chiffres.
// 
//----------------------------------------------------------------------------------------
//
// NOM: DIOUF       Prénom: Mamadou             Groupe: A3
//
//-----------------------------------------------------------------------------------------
