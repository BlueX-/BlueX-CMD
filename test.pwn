#include <a_samp>

stock bluex_strdel(string[],start,end)
{
  while( start != end)
  {
    string[start] = EOS;
    start++;
  }
}
