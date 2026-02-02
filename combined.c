	

    int main()
    {
        int *inv;
        int acc;
        int i;
        int sum;
        int j;
        int n;

        n = set_n();
        acc = set_acc();
        inv = alloc_inv();
        while(1) 
	    {	
            some_calculations(inv);
            i = n  - 2;
            while(i >= 0 && inv[i] == 0)
            {
                i--;
            }
            if(i < 0)
                break;
            inv[i] = inv[i] - acc;		
            sum = 0;
            j = i + 1;
            while(j < n)
            {
                sum += inv[j];
                inv[j] = 0;
                j++;
            }
            if (i + 1 < n)
                inv[i+1] = sum + acc;
        }
    }