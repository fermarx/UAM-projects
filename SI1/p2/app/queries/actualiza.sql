-- CHANGING THE GIVEN TABLES
    --Changes in customers table
    ALTER TABLE customers ADD PRIMARY KEY (customerid);
    ALTER TABLE customers ADD CONSTRAINT emailUniq UNIQUE (email);

    --Changes in imdb_actormovies table
    ALTER TABLE imdb_actormovies ADD CONSTRAINT actormov_pkey
    PRIMARY KEY (actorid, movieid);
    ALTER TABLE imdb_actormovies ADD CONSTRAINT actormov_aid_fkey
    FOREIGN KEY (actorid) ON DELETE CASCADE;
    ALTER TABLE imdb_actormovies ADD CONSTRAINT actormov_mid_fkey
    FOREIGN KEY (movieid) ON DELETE CASCADE;

    --Changes in imdb_actors table
    ALTER TABLE imdb_actors PRIMARY KEY (actorid);
    ALTER TABLE imdb_actors ADD UNIQUE (actorid);

    --Changes in imdb_directormovies table
    ALTER TABLE imdb_directormovies
    DROP CONSTRAINT dmov_pkey;

    ALTER TABLE imdb_directormovies ADD CONSTRAINT dmov_pkey
    PRIMARY KEY (directorid, movieid);

    --Changes in imdb_directors table
    ALTER TABLE imdb_directors PRIMARY KEY (directorid);
    ALTER TABLE imdb_directors ADD UNIQUE (directorid);

    --Changes in imdb_movies table
    ALTER TABLE imdb_movies PRIMARY KEY (movieid);
    ALTER TABLE imdb_movies ADD UNIQUE (movieid);

    --Changes in imdb_moviecountries table
    ALTER TABLE imdb_moviecountries FOREIGN KEY (movieid);

    --Changes in imdb_moviegenres table
    ALTER TABLE imdb_moviegenres FOREIGN KEY (movieid);

    --Changes in imdb_movielanguages table
    ALTER TABLE imdb_movielanguages FOREIGN KEY (movieid);

    --Changes in inventory table
    ALTER TABLE inventory FOREIGN KEY (prod_id);

    --Changes in orderdetail table 

    --WE create an auxiliary table to sum the price of two products with the same prod_id and order_id
    CREATE TABLE
        orderdetailb
    AS
      SELECT
        orderid,
        prod_id,
        SUM(quant) AS quant
      FROM
        orderdetail
      GROUP BY
        orderid,
        prod_id
      HAVING
        COUNT(concat(prod_id,'-',orderid))>1;

    DELETE
        FROM
            orderdetail
        WHERE
            EXISTS (
                SELECT *
                FROM
                orderdetailb AS o
                WHERE
                o.orderid = orderdetail.orderid AND
                o.prod_id = orderdetail.prod_id
            );

    INSERT INTO
        orderdetail(orderid, prod_id, quant)
        SELECT *
        FROM
        orderdetailb;

    DROP TABLE orderdetailaux;

    ALTER TABLE orderdetail ADD ADD CONSTRAINT orderdetail_pkey
    PRIMARY KEY (orderid, prod_id);
    ALTER TABLE orderdetail ADD CONSTRAINT orderdetail_oid_fkey
    FOREIGN KEY (orderid) REFERENCES orders(orderid) ON DELETE CASCADE; 
    ALTER TABLE orderdetail ADD CONSTRAINT orderdetail_pid_fkey
    FOREIGN KEY (prod_id) REFERENCES products(prod_id); 

    --Suma de cantidades por pedido
    UPDATE products
    SET
        sales = summ.sales
    FROM (
        SELECT
            prod_id,
            sum(quantity)
        AS
            sales
        FROM
            orderdetail
        GROUP BY
            prod_id
        )   AS summ
    WHERE
        summ.prod_id = products.prod_id;

    --Changes in orders table
    ALTER TABLE orders PRIMARY KEY (orderid);
    ALTER TABLE orders ADD CONSTRAINT orders_cid_fkey;
    FOREIGN KEY (customerid) REFERENCES customers(customerid);

    --Changes in products table
    ALTER TABLE products PRIMARY KEY (prod_id);
    ALTER TABLE products FOREIGN KEY (movieid);
    ALTER TABLE products
        ADD COLUMN products_stock NUMERIC
            DEFAULT 10;
    ALTER TABLE products
        ADD COLUMN products_sales NUMERIC
            DEFAULT 0;


-- IMDB_MOVIECOUNTRIES
    -- Creating countries table
    CREATE TABLE public.countries(
        country_id integer PRIMARY KEY NOT NULL,
        country character varying(32) NOT NULL
    );
    ALTER TABLE public.countries OWNER TO alumnodb;

    -- Creating sequence of the id (PK) before inserting
    CREATE SEQUENCE public.countries_country_id_seq
        START WITH 1
        INCREMENT BY 1
        NO MINVALUE
        NO MAXVALUE
        CACHE 1;
    ALTER TABLE public.countries_country_id_seq OWNER TO alumnodb;

    ALTER SEQUENCE public.countries_country_id_seq OWNED BY public.countries.country_id;

    ALTER TABLE ONLY public.countries ALTER COLUMN country_id SET DEFAULT nextval('public.countries_country_id_seq'::regclass);

    -- countries
    INSERT INTO public.countries(country)
    SELECT DISTINCT country
    FROM public.imdb_moviecountries;

    -- language_id column
    ALTER TABLE public.imdb_moviecountries
    ADD COLUMN country_id integer;

    -- Updating column with countries ids
    UPDATE imdb_moviecountries
    SET country_id=countries.country_id
    FROM countries
    WHERE imdb_moviecountries.country=countries.country;

    -- Deleting column with countries strings
    ALTER TABLE imdb_moviecountries
    DROP COLUMN country;

    -- Setting up new PK's and FK's
    ALTER TABLE ONLY public.imdb_moviecountries
    ADD CONSTRAINT imdb_moviecountries_pkey PRIMARY KEY (movieid, country_id);

    ALTER TABLE ONLY public.imdb_moviecountries
    ADD CONSTRAINT imdb_moviecountries_country_id_fkey FOREIGN KEY (country_id) REFERENCES public.countries(country_id);


-- IMDB_MOVIEGENRES
    -- Creating genres table
    CREATE TABLE public.genres(
        genre_id integer PRIMARY KEY NOT NULL,
        genre character varying(32) NOT NULL
    );
    ALTER TABLE public.genres OWNER TO alumnodb;

    -- Creating sequence of the id (PK) before inserting
    CREATE SEQUENCE public.genres_genre_id_seq
        START WITH 1
        INCREMENT BY 1
        NO MINVALUE
        NO MAXVALUE
        CACHE 1;
    ALTER TABLE public.genres_genre_id_seq OWNER TO alumnodb;

    ALTER SEQUENCE public.genres_genre_id_seq OWNED BY public.genres.genre_id;

    ALTER TABLE ONLY public.genres ALTER COLUMN genre_id SET DEFAULT nextval('public.genres_genre_id_seq'::regclass);

    -- Inserting genres
    INSERT INTO public.genres(genre)
    SELECT DISTINCT genre
    FROM public.imdb_moviegenres;

    -- Adding new genre_id column
    ALTER TABLE public.imdb_moviegenres
    ADD COLUMN genre_id integer;

    -- Updating column with genres ids
    UPDATE imdb_moviegenres
    SET genre_id=genres.genre_id
    FROM genres
    WHERE imdb_moviegenres.genre=genres.genre;

    -- Deleting column with genres strings
    ALTER TABLE imdb_moviegenres
    DROP COLUMN genre;

    -- Setting up new PK's and FK's
    ALTER TABLE ONLY public.imdb_moviegenres
    ADD CONSTRAINT imdb_moviegenres_pkey PRIMARY KEY (movieid, genre_id);

    ALTER TABLE ONLY public.imdb_moviegenres
    ADD CONSTRAINT imdb_moviegenres_genre_id_fkey FOREIGN KEY (genre_id) REFERENCES public.genres(genre_id);


-- IMDB_MOVIELANGUAGES
    -- Creating new languages table
    CREATE TABLE public.languages(
        language_id integer PRIMARY KEY NOT NULL,
        language character varying(32) NOT NULL
    );
    ALTER TABLE public.languages OWNER TO alumnodb;

    -- Creating sequence of the id (PK) before inserting
    CREATE SEQUENCE public.languages_language_id_seq
        START WITH 1
        INCREMENT BY 1
        NO MINVALUE
        NO MAXVALUE
        CACHE 1;
    ALTER TABLE public.languages_language_id_seq OWNER TO alumnodb;

    ALTER SEQUENCE public.languages_language_id_seq OWNED BY public.languages.language_id;

    ALTER TABLE ONLY public.languages ALTER COLUMN language_id SET DEFAULT nextval('public.languages_language_id_seq'::regclass);

    -- Inserting languages
    INSERT INTO public.languages(language)
    SELECT DISTINCT language
    FROM public.imdb_movielanguages;

    -- Adding new language_id column
    ALTER TABLE public.imdb_movielanguages
    ADD COLUMN language_id integer;

    -- Updating column with languages ids
    UPDATE imdb_movielanguages
    SET language_id=languages.language_id
    FROM languages
    WHERE imdb_movielanguages.language=languages.language;

    -- Deleting column with languages strings
    ALTER TABLE imdb_movielanguages
    DROP COLUMN language;

    -- Setting up new PK's and FK's
    ALTER TABLE ONLY public.imdb_movielanguages
    ADD CONSTRAINT imdb_movielanguages_pkey PRIMARY KEY (movieid, language_id);

    ALTER TABLE ONLY public.imdb_movielanguages
    ADD CONSTRAINT imdb_movielanguages_language_id_fkey FOREIGN KEY (language_id) REFERENCES public.languages(language_id);


-- CREATING NEW ALERTAS TABLE
    CREATE TABLE public.alerts(
        prod_id integer PRIMARY KEY NOT NULL
    );
    ALTER TABLE public.alerts OWNER TO alumnodb;

    ALTER TABLE ONLY public.alerts
    ADD CONSTRAINT alertas_prod_id_fkey FOREIGN KEY (prod_id) REFERENCES public.products(prod_id);
