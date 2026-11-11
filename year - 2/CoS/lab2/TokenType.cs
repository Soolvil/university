using System.Text.RegularExpressions;

namespace TextParsing{
static class TokenType{
   public static bool IsBlank(char symbol){
      switch (symbol){
         case ' ':
         case '\n':
         return true;
         default: return false;
      }
   }
   public static bool IsDelimiter(char symbol){
      switch (symbol){
         case '.':return true;
         case ',':return true;
         case '!':return true;
         case '?':return true;
         case ':':return true;
         case ';':return true;
         case '\'':return true;
         case '\"':return true;
         default: return IsBlank(symbol);
      }
   }
}
} // namespace TextParsing
