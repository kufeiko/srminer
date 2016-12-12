
Configuration @ admin/config/services/srminer

Sample query:

$query = array(
  'type' => 'get',
  'entity' => 'ContactSet',
  'parameters' => array(
    '$filter' => array(
      'or' => array(
        'startswith' => array(
          'Name',"'Smith'",
        ),
        'eq' => array(
          'Type', 'Driver',
        ),
      ),
    ),
    '$select' => array(
      'field1',
      'field2',
    ),
    '$top' => 5,
    '$skip' => 15,
    '$orderby' => array(
      'desc' => array(
        'field1',
        'field2'
      ),
    ),
  ),
);

This should compile to:
$filter=startswith(Name,'Smith') or (Type eq Driver)&$select=field1,field2&$top=5&$skip15&$orderby=field1,field2 desc

The parameters are passed "as is", that means if you have to put quotes around a string - the query builder will not do it for you.