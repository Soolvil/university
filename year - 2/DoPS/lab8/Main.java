// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

record ParsingResult(String result, int successes, int failures) {};

class LectureJournalParser{
   String sourceString;
   static private String[] processStream(String rawStream){
      Pattern separator = Pattern.compile("//");
      return separator.split(rawStream, 256);
   }
   static private String validateStudentId(String idStr){
      Pattern pattern = Pattern.compile("^\\s*СТУД-(\\d{5})\\s*(?!.)");
      Matcher matcher = pattern.matcher(idStr);
      if(!matcher.find())return "";
      return matcher.group(1); 
   }
   static private String extractMarkTime(String timeStr){
       Pattern pattern = Pattern.compile("^\\s*(?:ВРЕМЯ:)\\s+([0-2]\\d:[0-5]\\d)(?:\\s+МСК)?\\s*(?!.)");
       Matcher matcher = pattern.matcher(timeStr);
       if(!matcher.find())return "";
       return matcher.group(1);
   }
   static private String cleanStudentName(String rawName){
      Pattern pattern = Pattern.compile("^[ #_\\+]*(\\p{IsCyrillic}+)[ _#\\+]+(\\p{IsCyrillic}+)[ #_\\+]*(?!.)");
      Matcher matcher = pattern.matcher(rawName);
      if(!matcher.find())return "";
      StringBuilder firstName  = new StringBuilder(matcher.group(1).toLowerCase());
      StringBuilder secondName = new StringBuilder(matcher.group(2).toLowerCase());
      firstName.setCharAt(0, Character.toUpperCase(firstName.charAt(0)));
      secondName.setCharAt(0, Character.toUpperCase(secondName.charAt(0)));
      return firstName.toString() + "" + secondName.toString();
   }
   static private String processRecord(String reportString){
      Pattern separator = Pattern.compile(";");
      String[] parts = separator.split(reportString, 3);
      if(parts.length != 3)return "";
      String id   = validateStudentId(parts[0]);
      String time =   extractMarkTime(parts[1]);
      String name =  cleanStudentName(parts[2]);
      if(id.equals("") || time.equals("") || name.equals(""))return "";
      return id + ", " + time + ", " + name + ",";
   }
   public void loadString(String str){
      sourceString = str;
   }
   public ParsingResult buildReport(){
      int failures = 0;
      int successes = 0;
      StringBuilder resultBuffer = new StringBuilder();
      String[] records = processStream(sourceString);
      // System.out.println(records.length);
      for(String record : records){
         String parsedRecord = processRecord(record);
         if(parsedRecord.equals("")){
            failures++;
            continue;
         }
         successes++;
         resultBuffer.append(parsedRecord);
         resultBuffer.append("\n");
      }
      return new ParsingResult(resultBuffer.toString(), successes, failures);
   }
}

public class Main {
   public static boolean parseableInt(String str){
      try {
         Integer.parseInt(str);
         return true;
      }
      catch (NumberFormatException e){
         return false;
      }
   }
   
   public static boolean parseableInt(String str, Predicate<Integer> filter){
      try {
         return filter.test(Integer.parseInt(str));
      }
      catch (NumberFormatException e){
         return false;
      }
   }

   public static boolean parseableDouble(String str){
      try {
         Double.parseDouble(str);
         return true;
      }
      catch (NumberFormatException e){
         return false;
      }
   }

   public static boolean parseableDouble(String str, Predicate<Double> filter){
      try {
         return filter.test(Double.parseDouble(str));
      }
      catch (NumberFormatException e){
         return false;
      }
   }

   public static String filteredScan(Scanner scan, String scanMsg, String failMsg, Predicate<String> filter){
      String buffer;
      System.out.print(scanMsg);
      buffer = scan.nextLine().trim();
      while(!filter.test(buffer)){
         System.out.print(failMsg);
         buffer = scan.nextLine().trim();
      };
      return buffer;
   }
   public static void main(){
      Scanner input = new Scanner(System.in);
      System.out.println("Введите строку для парсинга:");
      String source = input.nextLine();
      LectureJournalParser parser = new LectureJournalParser();
      parser.loadString(source);
      ParsingResult res = parser.buildReport();
      System.out.print(res.result());
      System.out.println("Успешно обработано: " + res.successes() + ", Отрабраковано: " + res.failures());
   }
}
