using System.Collections;
using System.IO;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.Text.RegularExpressions;
using TextParsing;

public class Program{
   static void SortByWordCount(Text text){
      text.Sort(new WordCountSentenceComparer());
   }
   static void SortByLength(Text text){
      text.Sort(new LengthSentenceComparer());
   }
   static List<string> WordsByLenghtInInterrogative(Text text, int length){
      HashSet<string> words = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
      foreach(Sentence sentence in text){
         if(sentence.Type != SentenceType.Interrogative)continue;
         foreach(Word word in sentence){
            if(word.Value.Length == length){
               words.Add(word.Value);
            }
         }
      }
      return new List<string>(words);
   }
   static void RemoveWordsWithConsonantByLength(Text text, int length){
      foreach(Sentence sentence in text){
         for(int i = 0; i < sentence.Count; i++){
            if(!(sentence[i] is Word))continue;
            if(sentence[i].Value.Length != length)continue;
            sentence.RemoveAt(i);
            i--;
         }
      }
   }
   static void ReplaceWordsByLength(Sentence sentence, int length, string replacement){
      for(int i = 0; i < sentence.Count; i++){
         if(!(sentence[i] is Word))continue;
         if(sentence[i].Value.Length != length)continue;
         sentence[i] = new Word(replacement);
      }
   }
   static void RemoveStopWords(Text text, List<string> stopWordsList){
      HashSet<string> stopWords = new HashSet<string>();
      foreach(string word in stopWordsList){
         stopWords.Add(word);
      }
      foreach(Sentence sentence in text){
         for(int i = 0; i < sentence.Count; i++){
            if(!(sentence[i] is Word))continue;
            if(stopWords.Contains(sentence[i].Value)){
               sentence.RemoveAt(i);
               i--;
            }
         }
      }
   }
   class WordOccurances{
      int count;
      List<int> indices;
      public int Count{
         get{return count;}
         set{count = value;}
      }
      public List<int> Indices{
         get{return indices;}
         set{indices = value;}
      }
   }
   static Dictionary<string, WordOccurances> getConcordance(Text text){
      Dictionary<string, WordOccurances> concordance = new Dictionary<string, WordOccurances>();
      for(int i = 0; i < text.Count; i++){
         foreach(Token token in text[i]){
            if(!(token is Word))continue;
            String word = token.Value;
            if(concordance.ContainsKey(word)){
               concordance[word].Count++;
               int indexCount = concordance[word].Indices.Count;
               int lastIndex = -1;
               if(indexCount > 0){
                  lastIndex = concordance[word].Indices[indexCount - 1];
               }
               if(lastIndex == i)continue;
               concordance[word].Indices.Add(i); 
            }
         }
      }
      return concordance;
   }
   static void Main(){
   
   }
}
