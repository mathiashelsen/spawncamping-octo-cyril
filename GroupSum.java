public class GroupSum
{
    public static void main(String[] args)
    {
	int[] a = {2, 4, 8};
	System.out.println(groupSum(0, a, 10));
	System.out.println(groupSum(0, a, 14));
	System.out.println(groupSum(0, a, 9));
    }

    static private boolean groupSum(int start, int[] nums, int target)
    {
	// If the series has been handled, is what remains of the target 0?
	if( start >= nums.length) 
	    return( target == 0);
    
	// Assume that the start is included, can a series be formed?
	if( groupSum(start+1, nums, target-nums[start]) )
	    return true;

	// Assume that the start is not included, can a series be formed?
	if( groupSum(start+1, nums, target) )
	    return true;
    
	// If this element is included or not, no solution was found
	return false;
    }

    static private boolean groupSumClump(int start, int[] nums, int target)
    {
	if( start >= nums.length ) 
	    return ( target == 0 );

	// Check if this is part of series
	if( (start < (nums.length - 1)) && (nums[start] == nums[start+1]) )
	{
	    // Sum all the values together
	    int sum = 0;
	    int j = start;
	    while(nums[j] == nums[start])
	    {
		sum += nums[j];
		j++;
	    }

	    if(groupSumClump(j, nums, target-sum))
		return true;
        
	    if(groupSumClump(j, nums, target))
		return true;
        
	    return false;
	}
   
	if(groupSumClump(start+1, nums, target-nums[start]))
	    return true;
        
	if(groupSumClump(start+1, nums, target))
	    return true;
  
	return false;
    }
}
