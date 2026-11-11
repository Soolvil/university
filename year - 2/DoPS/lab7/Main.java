// Вариант 21
import java.util.Scanner;
import java.util.function.Predicate;

record OperationResult(boolean err, String opMsg) {};

enum SafetyLoopState{
   CLOSED_READY,
   OPEN_FAULT
}

abstract class MovingWalkway{
   boolean isMoving;
   SafetyLoopState safetyLoop;
   public MovingWalkway(){
      isMoving = false;
      safetyLoop = SafetyLoopState.CLOSED_READY;
   }
   SafetyLoopState getSafetyLoopState(){
      return safetyLoop;
   }
   public void resetSaferyLoop(){
      isMoving = false;
      safetyLoop = SafetyLoopState.CLOSED_READY;
   }
   boolean getMovingStatus(){
      return isMoving;
   }
   public OperationResult startMoving(){
      if(safetyLoop == SafetyLoopState.OPEN_FAULT)return new OperationResult(true, "Ошибка: движение невозможно, обрыв цепи безопасности.");
      isMoving = true;
      return new OperationResult(false, "");
   }
   public void stopMoving(){
      isMoving = false;
   }
   public String toString(){
      return "{isMoving:" + isMoving + ", SafetyLoop:" + safetyLoop + "}";
   }
}

class TunnelEscalator extends MovingWalkway{
   boolean brakeWeared;
   double railTension;
   public TunnelEscalator(boolean weared, double tension){
      super();
      brakeWeared = weared;
      railTension = tension;
   }
   public OperationResult testEmergencyBrake(){
      if(brakeWeared)return new OperationResult(true, "Ошибка: аварийный тормоз изношен.");
      return new OperationResult(false, "");
   }
   void setBrakeStatus(boolean weared){
      brakeWeared = weared;
   }
   boolean getBreakStatus(){
      return brakeWeared;
   }
   void setTension(double tension){
      railTension = tension;
   }
   double getTension(){
      return railTension;
   }
   @Override
   public String toString(){
      return "{isMoving:" + isMoving + ", safetyLoop:" + safetyLoop
       + ", brakeWeared:" + brakeWeared + ", railTension:" + railTension + "}";
   }
}

class FloorEscalator extends MovingWalkway{
   boolean standby;
   boolean hasPassengers;
   public FloorEscalator(){
      super();
      standby = false;
      hasPassengers = false;
   }
   public OperationResult toggleStandbyMode(){
      if(hasPassengers)return new OperationResult(true, "Ошибка: невозможно включить режим энергосбережения, есть пассажиры.");
      standby = true;
      return new OperationResult(false, "");
   }
   boolean getStandbyStatus(){
      return standby;
   }
   void setPassengersStatus(boolean passengersStatus){
      hasPassengers = passengersStatus;
   }
   boolean getPassengersStatus(){
      return hasPassengers;
   }
   @Override
   public String toString(){
      return "{isMoving:" + isMoving + ", safetyLoop:" + safetyLoop +
      ", standby:" + standby + ", hasPassengers:" + hasPassengers + "}";
   }
}

class HorizontalTravelator extends MovingWalkway{
   double length;
   double lubricationLevel;
   public HorizontalTravelator(double len, double level){
      super();
      length = len;
      lubricationLevel = level;
   }
   double getLength(){
      return length;
   }
   void setLength(double newLength){
      length = newLength;
   }
   double getLubricationLevel(){
      return lubricationLevel;
   }
   void setLubricationLevel(double newLubricationLevel){
      lubricationLevel = newLubricationLevel;
   }
   @Override
   public String toString(){
      return "{isMoving:" + isMoving + ", safetyLoop:" + safetyLoop
       + ", length:" + length + ", lubricationLevel:" + lubricationLevel + "}";
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
   }
}
