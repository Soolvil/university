using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.Collections;

namespace TextParsing{
public class WordCountSentenceComparer : IComparer<Sentence>{
   public int Compare(Sentence? first, Sentence? second){
      return first.WordCount.CompareTo(second.WordCount);
   }
}
public class LengthSentenceComparer : IComparer<Sentence>{
   public int Compare(Sentence? first, Sentence? second){
      return first.Length.CompareTo(second.Length);
   }
}

public enum SentenceType{
   Null,
   Declarative,
   Interrogative,
   Exclamation,
}
  
public class Sentence : IEnumerable<Token>{
   private List<Token> tokens;
   [XmlElement("token")]
   public Token this[int index]{
      get {return tokens[index];}
      set {tokens[index] = value;}
   }
   public Sentence(){
      tokens = new List<Token>();
   }
   public Token EndOfSentence{
      get {return tokens[tokens.Count - 1];}
   }
   public int WordCount{
      get {
         int counter = 0;
         foreach(Token t in tokens){
            if(t is Word)counter++;
         }
         return counter;
      }
   }
   public int PunctuationCount{
      get {
         int counter = 0;
         foreach(Token t in tokens){
            if(t is Punctuation)counter++;
         }
         return counter;
      }
   }
   public int Count{
      get {return tokens.Count;}
   }
   public int Length{
      get {
         int counter = 0;
         foreach(Token t in tokens){
          counter += t.Value.Length;
         }
         return counter;
      }
   }
   public SentenceType Type{
      get {
         switch (EndOfSentence.Value){
            case ".": return SentenceType.Declarative;
            case "?": return SentenceType.Interrogative;
            case "!": return SentenceType.Exclamation;
            default: return SentenceType.Null;
         }
      }
   }
   public void Add(Token token){
      tokens.Add(token);
   }
   public void Insert(int index, Token token){
      tokens.Insert(index, token);
   }
   public void RemoveAt(int index){
      tokens.RemoveAt(index);
   }
   public void Sort(IComparer<Token> cmp){
      tokens.Sort(cmp);
   }
   public void Clear(){
      tokens.Clear();
   }
   public Sentence Clone(){
      Sentence newSentence = new Sentence();
      newSentence.tokens = new List<Token>(this.Count);
      for(int i = 0; i < this.Count; i++)newSentence.Add(this[i].Clone());
      return newSentence;
   }
   public IEnumerator<Token> GetEnumerator(){
      return tokens.GetEnumerator(); 
   }
   IEnumerator IEnumerable.GetEnumerator(){
      return this.GetEnumerator();
   }
}
} // namespace TextParsing
