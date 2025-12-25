{
  "$schema": "https://vega.github.io/schema/vega-lite/v6.json",
  "background": "white",
  "width": {"step": 70},
  "height": {"step": 70},
  "data": {
    "values": [
      {"date":"2025-01-01","X1":100,"X2":50,"X3":80,"X4":120,"X5":60,"X6":30},
      {"date":"2025-01-02","X1":101,"X2":49,"X3":81,"X4":121,"X5":59,"X6":31},
      {"date":"2025-01-03","X1":103,"X2":50,"X3":82,"X4":123,"X5":58,"X6":32},
      {"date":"2025-01-04","X1":102,"X2":51,"X3":83,"X4":122,"X5":59,"X6":31},
      {"date":"2025-01-05","X1":104,"X2":52,"X3":82,"X4":125,"X5":60,"X6":33},
      {"date":"2025-01-06","X1":106,"X2":51,"X3":81,"X4":128,"X5":61,"X6":34},
      {"date":"2025-01-07","X1":105,"X2":50,"X3":80,"X4":127,"X5":62,"X6":35},
      {"date":"2025-01-08","X1":107,"X2":49,"X3":81,"X4":129,"X5":61,"X6":34},
      {"date":"2025-01-09","X1":108,"X2":50,"X3":82,"X4":130,"X5":60,"X6":33},
      {"date":"2025-01-10","X1":110,"X2":51,"X3":83,"X4":132,"X5":61,"X6":32},
      {"date":"2025-01-11","X1":109,"X2":52,"X3":84,"X4":131,"X5":62,"X6":31},
      {"date":"2025-01-12","X1":111,"X2":51,"X3":83,"X4":133,"X5":63,"X6":30},
      {"date":"2025-01-13","X1":112,"X2":50,"X3":82,"X4":134,"X5":64,"X6":29},
      {"date":"2025-01-14","X1":113,"X2":49,"X3":81,"X4":135,"X5":63,"X6":28},
      {"date":"2025-01-15","X1":115,"X2":50,"X3":82,"X4":137,"X5":62,"X6":29},
      {"date":"2025-01-16","X1":114,"X2":51,"X3":83,"X4":136,"X5":61,"X6":30},
      {"date":"2025-01-17","X1":116,"X2":52,"X3":84,"X4":138,"X5":60,"X6":31},
      {"date":"2025-01-18","X1":118,"X2":51,"X3":85,"X4":140,"X5":61,"X6":32},
      {"date":"2025-01-19","X1":117,"X2":50,"X3":84,"X4":139,"X5":62,"X6":33},
      {"date":"2025-01-20","X1":119,"X2":49,"X3":83,"X4":141,"X5":63,"X6":34}
    ]
  },
  "transform": [
    {"fold": ["X1","X2","X3","X4","X5","X6"], "as": ["var","price"]},
    {
      "window": [{"op":"lag","field":"price","as":"prev"}],
      "sort": [{"field":"date"}],
      "groupby": ["var"]
    },
    {"filter": "isValid(datum.prev)"},
    {"calculate": "log(datum.price / datum.prev)", "as": "ret"},
    {"pivot": "var", "value": "ret", "groupby": ["date"]},
    {
      "calculate": "[( {key:'X1–X4',col:1,row:1,A:datum.X1,B:datum.X4} ),( {key:'X3–X5',col:2,row:1,A:datum.X3,B:datum.X5} ),( {key:'X2–X6',col:3,row:1,A:datum.X2,B:datum.X6} ),( {key:'X3–X2',col:4,row:1,A:datum.X3,B:datum.X2} )]",
      "as": "pairs"
    },
    {"flatten": ["pairs"], "as": ["p"]},
    {"calculate": "datum.p.key", "as": "key"},
    {"calculate": "datum.p.col", "as": "col"},
    {"calculate": "datum.p.row", "as": "row"},
    {"calculate": "datum.p.A", "as": "A"},
    {"calculate": "datum.p.B", "as": "B"},
    {"filter": "isValid(datum.A) && isValid(datum.B)"},
    {"calculate": "datum.A*datum.B", "as":"AB"},
    {"calculate": "datum.A*datum.A", "as":"A2"},
    {"calculate": "datum.B*datum.B", "as":"B2"},
    {
      "window": [
        {"op":"count","as":"n3"},
        {"op":"sum","field":"A","as":"sA"},
        {"op":"sum","field":"B","as":"sB"},
        {"op":"sum","field":"AB","as":"sAB"},
        {"op":"sum","field":"A2","as":"sA2"},
        {"op":"sum","field":"B2","as":"sB2"}
      ],
      "frame": [-2, 0],
      "sort": [{"field":"date"}],
      "groupby": ["key","col","row"]
    },
    {
      "calculate": "datum.n3==3 ? ((datum.n3*datum.sAB - datum.sA*datum.sB) / sqrt((datum.n3*datum.sA2 - pow(datum.sA,2))*(datum.n3*datum.sB2 - pow(datum.sB,2)))) : null",
      "as": "corr3"
    },
    {
      "window": [{"op":"row_number","as":"rn"}],
      "sort": [{"field":"date","order":"descending"}],
      "groupby": ["key","col","row"]
    },
    {"filter": "datum.rn==1"},
    {"calculate": "format(datum.corr3, '.3f')", "as":"corr_fmt"}
  ],
  "encoding": {
    "x": {
      "field": "col",
      "type": "ordinal",
      "axis": null,
      "scale": {"domain":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18]}
    },
    "y": {
      "field": "row",
      "type": "ordinal",
      "axis": null,
      "scale": {"domain":[1,2,3,4,5,6,7], "reverse": true}
    }
  },
  "layer": [
    {
      "mark": {"type":"rect","cornerRadius": 10,"strokeWidth": 1},
      "encoding": {"fill":{"value":"#ffffff"}, "stroke":{"value":"#cfcfcf"}}
    },
    {
      "mark": {"type":"text","align":"left","baseline":"top","dx": -28,"dy": -28,"fontSize": 14,"fontWeight":"bold","fill":"#111"},
      "encoding": {"text":{"field":"key"}}
    },
    {
      "mark": {"type":"text","fontSize": 24,"fontWeight":"bold","dy": 10,"fill":"#111"},
      "encoding": {"text":{"field":"corr_fmt"}}
    }
  ],
  "config": {"view":{"stroke": null}, "axis":{"grid": false}}
}
