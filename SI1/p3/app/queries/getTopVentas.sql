DROP FUNCTION IF EXISTS getTopVentas(integer,integer);
CREATE OR REPLACE FUNCTION getTopVentas(year1 int, year2 int)
  RETURNS TABLE (
    agno INT,
    pelicula VARCHAR,
    ventas BIGINT
  )
  AS $$

  BEGIN
    RETURN QUERY
    SELECT
      agno,
      t.pelicula,
      t.ventas
    FROM(
      SELECT
        date_part('year', orders.orderdate)::INTEGER AS agno,
        imdb_movies.movietitle AS pelicula,
        SUM(orderdetail.quantity)::INTEGER AS ventas,
        products.movieid AS movieid
      FROM
        orders
          INNER JOIN
        orderdetail ON orderdetail.orderid = orders.orderid
          INNER JOIN
        products ON products.prod_id = orderdetail.prod_id
          INNER JOIN
        imdb_movies ON imdb_movies.movieid = products.movieid
      WHERE
        date_part('year', orders.orderdate) >= year1 AND date_part('year', orders.orderdate) <= year2
      GROUP BY
        products.prod_id,
        agno,
        imdb_movies.movietitle
      ORDER BY
        agno,
		    pelicula,
        ventas DESC
    ) AS t;
  END; $$ LANGUAGE PLPGSQL;

SELECT * FROM getTopVentas(2014, 2020)
