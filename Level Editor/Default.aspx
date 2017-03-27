<%@ Page Language="VB" AutoEventWireup="false" CodeFile="Default.aspx.vb" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <title></title>
    <style>
        #divEditor {
            width: 65%;
            height: 1200px;
            border: 5px solid;
            overflow-x: scroll;
            overflow-y: scroll;
            float: left;
        }

       table {
           border-spacing: 0px;
       }

       .tile {

       }


    </style>
</head>
<body>
    <form id="form1" runat="server">
        <div id="divEditor">
            <table id="tableTiles">
                <tbody>

                </tbody>
            </table>
        </div>
        <div id="divControls" style="float:right;width:30%">
            <h2>Controls</h2>
            <table>
                <tr>
                    <td>
                        <label>Select Tile</label>
                    </td>
                    <td>
                        <select id="tileSelector">
                            <option value="1">1</option>
                            <option value="2">2</option>
                            <option value="3">3</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td>
                        <label>Tile Type</label>
                    </td>
                    <td>
                        <select>
                            <option value="1">Tile</option>
                            <option value="2">Collider</option>
                        </select>
                    </td>
                </tr>
            </table>
        </div>
    </form>
</body>
</html>

<script>
    $(document).ready(function () {
        for (var i = 0; i < 40; i++) {
            $('#tableTiles tbody').append('<tr></tr>');
            for (var j = 0; j < 40; j++) {
                $("#tableTiles tr:last").append("<td><div class='tile' style='width:64px;height:64px;border:2px solid;' id='tile_" + i + "_" + j + "'></div><td>");
            }
        }

        $('.tile').click(function (event) {
            var tileID = $('#tileSelector').val();
            switch (tileID)
            {
                case '1':
                    $(this).css("background", 'url("images/tileset_platforms.png") 0 0');
                    break;
                case '2':
                    $(this).css("background", 'url("images/tileset_platforms.png") 74px 0');
                    break;
                case '3':

                    $(this).css("background", 'url("images/tileset_platforms.png") 148px 0px');
                    break;
            }   
        });
    });

    

</script>