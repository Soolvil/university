// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

record OperationResult(boolean err, String opMsg) {};

class ShipEngine{
   double coords;
   public void move(double distance) {
      coords += distance;
   }
   public double getCoordinates(){
      return coords;
   }
}
   
enum AnchorStatus{
   LIFTED,
   DROPPED
}

class ShipBody{
   private ShipEngine engine;
   private AnchorStatus anchorStatus;
   public ShipBody(ShipEngine _engine){
      engine = _engine;
      anchorStatus = AnchorStatus.LIFTED;
   }
   public ShipEngine getEngine(){
      return engine;
   }
   public void setEngine(ShipEngine newEngine){
      engine = newEngine;
   }
   public AnchorStatus getAnchorStatus(){
      return anchorStatus;
   }
   public OperationResult dropAnchor(){
      return switch(anchorStatus){
	 case LIFTED -> {
	    anchorStatus = AnchorStatus.DROPPED;
	    yield new OperationResult(false, "Якорь сброшен.");
	 }
	 case DROPPED -> new OperationResult(true, "Ошибка: якорь уже сброшен.");
      };
   }
   public OperationResult liftAnchor(){
      return switch(anchorStatus){
	 case LIFTED -> new OperationResult(true, "Ошибка: якорь уже поднят.");
	 case DROPPED -> {
	    anchorStatus = AnchorStatus.LIFTED;
	    yield new OperationResult(false, "Якорь поднят.");
	 }
      }; 
   }
}

class ShipRudder{
   public OperationResult move(double distance, ShipEngine engine, AnchorStatus anchorStatus){
      if(anchorStatus == AnchorStatus.DROPPED){
	 return new OperationResult(true, "Ошибка: якорь опущен, движение невозможно.");
      }
      engine.move(distance);
      return new OperationResult(false, "Движение завершено.");
   }
}

class Ship{
   private ShipBody body;
   private ShipRudder rudder;
   public Ship(ShipBody _body, ShipRudder _rudder){
      body = _body;
      rudder = _rudder;
   }
   public double getCoordinates(){
      return body.getEngine().getCoordinates();
   }
   public OperationResult move(double distance){
      return rudder.move(distance, body.getEngine(), body.getAnchorStatus());
   }
   public AnchorStatus getAnchorStatus(){
      return body.getAnchorStatus();
   }
   public OperationResult dropAnchor(){
      return body.dropAnchor();
   }
   public OperationResult liftAnchor(){
      return body.liftAnchor();
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
      Ship ship = new Ship(new ShipBody(new ShipEngine()), new ShipRudder());
      while(true){
	 Scanner input = new Scanner(System.in);
	 int opCode = 0;
	 opCode = Integer.parseInt(filteredScan(input,
	    "Введите код операции:",
	    "Код операции должен быть целым числом.\nВведите ещё раз:",
	    (String str) -> parseableInt(str)));
	 if(opCode == 0){
	    input.close();
	    break;
	 }
	 switch (opCode){
	    case 1:
	       System.out.print("Текущий статус якоря:");
	       System.out.println(
		     switch(ship.getAnchorStatus()){
			case LIFTED -> "Поднят";
			case DROPPED -> "Сброшен";
		     });
	       break;
	    case 2:
	       System.out.println(ship.liftAnchor().opMsg());
	       break;
	    case 3:
	       System.out.println(ship.dropAnchor().opMsg());
	       break;
	    case 4:
	       System.out.println("Текущие координаты корабля:" + ship.getCoordinates());
	       break;
	    case 5:
	       double distance = Double.parseDouble(filteredScan(input,
		  "Введите на какое расстояние сдвинуться:",
		  "Расстояние должно быть неотрицательным числом.\nВведите ещё раз:",
		  (String str) -> parseableDouble(str, (Double d) -> d >= 0)));
	       System.out.println(ship.move(distance).opMsg());
	       break;
	    default:
	       System.out.println("Число " + opCode + " не является кодом ни одной операции, повторите свой ввод.");
	 }
      }
   }
}
