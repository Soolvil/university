using System.Text.RegularExpressions;

namespace TextParsing{
   public class TextParser{
      private Text textBuff;
      private Sentence sentenceBuff;
      private String tokenBuff;
      public TextParser() {
	 textBuff = new Text();
	 sentenceBuff = new Sentence();
	 tokenBuff = new String("");
      }
      public Text Value{
	 get{return textBuff;}
      }
      private Token MakeToken(){
	 String tokenVal = tokenBuff; 
	 tokenBuff = new String("");
	 if(Word.IsWord(tokenVal)){
	    return new Word(tokenVal);
	 }
	 else{
	    return new Punctuation(tokenVal);
	 }
      }
      private void ExtendSentence(Token token){
	 sentenceBuff.Add(token);
	 if(Regex.IsMatch(sentenceBuff.EndOfSentence.Value, "[.?!]")){
	    textBuff.Add(sentenceBuff);
	    sentenceBuff = new Sentence();
	 }
      }
      public void ParseChar(char symbol){
	 if(TokenType.IsDelimiter(symbol)){
	    ExtendSentence(this.MakeToken());  
	 }
	 if(!TokenType.IsBlank(symbol)){
	    tokenBuff += symbol;
	 }
      }
      public void ParseString(String source){
	 foreach(char symbol in source)ParseChar(symbol);
	 if(tokenBuff.Length != 0)this.ExtendSentence(this.MakeToken());
      }
   }
}
