package util;

public class Chances {
    public static Integer randomInteger(Integer min, Integer max){
        Double val = Math.random() * (max - min + 1) + min;
        return val.intValue();
    }

    public static Double randomDouble(Double min, Double max){
        return Math.random() * (max - min) + min;
    }

    //From 0.0 (0%) to 100.0 (100%)
    public static Boolean randomBoolean(Double chance){
        if(randomDouble(0.0, 100.0) < chance) return true;
        return false;
    }
}
