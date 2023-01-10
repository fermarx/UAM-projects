ALTER TABLE customers ADD promo SMALLINT CONSTRAINT valid CHECK(promo >= 0 AND promo <= 100);

CREATE OR REPLACE FUNCTION updP() RETURNS TRIGGER AS $$
  BEGIN
    UPDATE orderdetail
    SET
      price = products.price - (products.price*(new.promo/100))
    FROM
      customers NATURAL JOIN orders NATURAL JOIN orderdetail NATURAL JOIN products
    WHERE
      orders.customerid = new.customerid
    AND
      orders.status is null;

    PERFORM pg_sleep(10);

    UPDATE orders
    SET
      netamount = calc.total,
      totalamount = ROUND((calc.total*(tax+100)/100), 2)
    FROM
      (SELECT
        orderid,
        SUM(price) AS total
      FROM
        orderdetail
      GROUP BY
        orderid) AS calc
    WHERE orders.orderid = calc.orderid;
  RETURN new;
  END;
$$ LANGUAGE PLPGSQL;

CREATE TRIGGER updPromo AFTER UPDATE ON customers
FOR EACH ROW EXECUTE PROCEDURE updP();
