using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace TextParsing{
[XmlInclude(typeof(Word)), XmlInclude(typeof(Punctuation))]
public abstract class Token{
   public abstract String Value{
      get; set;
   }
   public abstract Token Clone();
}
} // namespace TextParsing
