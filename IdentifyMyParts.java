import Point.*;

public class IdentifyMyParts {
    public static int x = 7; 
    public int y = 3; 

    public static void main(String[] args)
    {
	Point a = new Point(1, 1, 0);
	Point b = new Point(0, 1, 0);
	System.out.print("Point a is at " + a.getX() + ", " + a.getY() + ", " + a.getZ());
    };
}


