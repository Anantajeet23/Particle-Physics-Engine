namespace lancelot {
    /**
     * The eingine is set to single precision by deafult
    */
   typedef float real;

    /*Setting the precision of sqrt function*/
   #define real_sqrt sqrtf

   /** Defines the precision of the power operator. */
    #define real_pow powf

    /** Defines the precision of the absolute magnitude operator. */
    #define real_abs fabsf      
}