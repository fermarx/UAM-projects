--CREATE INDEX month_idx ON orders(date_part('month', orderdate));
--CREATE INDEX year_idx ON orders(date_part('year', orderdate));
--CREATE INDEX totala_idx ON orders(totalamount);

CREATE INDEX myt_idx ON orders(date_part('year', orderdate), date_part('month', orderdate), totalamount);

EXPLAIN SELECT
  COUNT(DISTINCT customerid)
FROM
  orders
WHERE
  date_part('year', orderdate) = 2015 AND --Durante el año 2014
  date_part('month', orderdate) = 04 AND  --Durante el mes de abril
  totalamount > 100; --Si se quiere cambiar el importe mínimo, cambiar el 100
