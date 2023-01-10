CREATE OR REPLACE FUNCTION setOrderAmount()
    RETURNS void AS $$
    BEGIN
        -- netamount
        UPDATE orders
        SET netamount=S.sum_price
        FROM (
            SELECT orderid, sum(price) AS sum_price
            FROM orderdetail
            GROUP BY orderid
        ) S
        WHERE orders.orderid=S.orderid;

        -- totalamount
        UPDATE orders
        SET totalamount=T.ta
        FROM (
            SELECT orderid, ROUND(
                                ((netamount*(tax/100)) + netamount)
                                , 2) AS ta
            FROM orders
        ) T
        WHERE orders.orderid=T.orderid;
    END;
$$ LANGUAGE plpgsql;