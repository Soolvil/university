using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.Collections;

namespace TextParsing{
[XmlRoot("text")]
public class Text : IEnumerable<Sentence>{
   private List<Sentence> sentences;
   [XmlElement("sentence")]
   public Sentence this[int index]{
      get{return sentences[index];}
      set{sentences[index] = value;}
   }
   public Text(){
      sentences = new List<Sentence>();
   }
   public int Count{
      get {return sentences.Count;}
   }
   public int WordCount{
      get {
         int counter = 0;
         foreach(Sentence sentence in sentences){
            counter += sentence.WordCount;
         }
         return counter;
      }
   }
   public int Length{
      get {
         int counter = 0;
         foreach(Sentence sentence in sentences){
            counter += sentence.Length;
         }
         return counter;
         }
   }
   public void Add(Sentence sentence){
      sentences.Add(sentence);
   }
   public void Insert(int index, Sentence sentence){
      sentences.Insert(index, sentence);
   }
   public void RemoveAt(int index){
      sentences.RemoveAt(index);
   }
   public void Sort(IComparer<Sentence> cmp){
      sentences.Sort(cmp);
   }
   public void Clear(){
      sentences.Clear();
   }
   public Text Clone(){
      Text newText = new Text();
      newText.sentences = new List<Sentence>(this.Count);
      for(int i = 0; i < this.Count; i++)newText.Add(this[i].Clone());
      return newText;
   }
   public IEnumerator<Sentence> GetEnumerator(){
      return sentences.GetEnumerator();
   }
   IEnumerator IEnumerable.GetEnumerator(){
      return this.GetEnumerator();
   }
}
} // namespace TextParsing
