-- integer 1 : product amount
-- integer 2 : totalamount
CREATE OR REPLACE FUNCTION getTopMonths(integer, integer)
    RETURNS TABLE (
        month integer,
        year integer,
        total_prod bigint,
        total_cash numeric
    )
    AS $$
    BEGIN
        RETURN QUERY SELECT
            CAST(m AS integer),
            CAST(yr AS integer),
            t_prod,
            t_cash
        FROM (
                SELECT date_part('month', orderdate) as m,
                       date_part('year', orderdate) as yr,
                       sum(quantity) as t_prod
                FROM orders INNER JOIN orderdetail
                    ON orders.orderid=orderdetail.orderid
                GROUP BY m, yr
            ) TotalProd
        NATURAL JOIN
            (
                SELECT date_part('month', orderdate) as m,
                       date_part('year', orderdate) as yr,
                       sum(totalamount) as t_cash
                FROM orders
                GROUP BY m, yr
            ) TotalCash
        WHERE (t_prod > $1)
             OR (t_cash > $2);
    END;
$$ LANGUAGE plpgsql;
