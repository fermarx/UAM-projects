-- without indexes
EXPLAIN
select count(*)
from orders
where status is null;

EXPLAIN
select count(*)
from orders
where status ='Shipped';

CREATE INDEX status_orders on orders(status);

-- with indexes
EXPLAIN
select count(*)
from orders
where status is null;

EXPLAIN
select count(*)
from orders
where status ='Shipped';

-- executing analyze on orders
ANALYZE;

-- after executing analyze
EXPLAIN
select count(*)
from orders
where status is null;

EXPLAIN
select count(*)
from orders
where status ='Shipped';

-- Other two queries of apendix 2
EXPLAIN 
select count(*)
from orders
where status ='Paid';

EXPLAIN
select count(*)
from orders
where status ='Processed';