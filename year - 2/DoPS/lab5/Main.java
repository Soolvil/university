// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

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

   public static record OperationResult(boolean err, String opMsg) {};

   public static class WatchPlayer {
      private int volume;
      private boolean isHeadphonesConnected;
      private String currentTrackName;
      WatchPlayer(){}
      WatchPlayer(String trackName){
	 this.currentTrackName = trackName;
      }
      public int getVolume(){
	 return this.volume;
      }
      public OperationResult setVolume(int targetVolume){
	 if(!this.isHeadphonesConnected){
	    return new OperationResult(true, "Ошибка: наушники не подключены, регулировка громкости невозможна.");
	 }
	 if(targetVolume < 0 || targetVolume > 10){
	    return new OperationResult(true, "Ошибка: уровень громкости " + targetVolume + " вне рамок диапазона 0-10");
	 }
	 this.volume = targetVolume;
	 if(this.volume < 10){
	    return new OperationResult(false, "Новая громкость: " + this.volume);
	 }
	 else {
	    return new OperationResult(false, "Новая громкость: " + this.volume + "\nПредупреждение: высокая громкость вредная для слуха.");
	 }
      }
      public boolean getConnectionStatus(){
	 return this.isHeadphonesConnected;
      }
      public OperationResult connectHeadphones(){
	 if(this.isHeadphonesConnected){
	    this.volume = 0;
	 }
	 this.isHeadphonesConnected = !this.isHeadphonesConnected;
	 return new OperationResult(false ,"Статус подключения наушников: " + this.isHeadphonesConnected);
      }
   }  

   public static void main(){
      WatchPlayer player = new WatchPlayer("Song 1");
      while(true){
	 Scanner input = new Scanner(System.in);
	 int opCode = 0;
	 opCode = Integer.parseInt(filteredScan(input,
	    "Введите код операции:",
	    "Код операции должен быть целым числом.\nВведите ещё раз:",
	    (String str) -> parseableInt(str)));
	 if(opCode == 0)break;
	 switch (opCode){
	    case 1:
	       System.out.println("Изменение статуса подключения наушников.");
	       System.out.println(player.connectHeadphones().opMsg());
	       break;
	    case 2:
	       int volume = Integer.parseInt(filteredScan(input,
		  "Введите желаемый уровень громкости:",
		  "Уровень громкости должен быть целым числом от 0 до 10.\nВведите ещё раз:",
		  (String str) -> parseableInt(str, (Integer num) -> num >= 0 && num <= 10)));
	       System.out.println(player.setVolume(volume).opMsg());
	       break;
	    default:
	       System.out.println("Число " + opCode + " не является кодом ни одной операции, повторите свой ввод.");
	 }
      }
   }
}
