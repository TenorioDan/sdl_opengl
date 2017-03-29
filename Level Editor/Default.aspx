<%@ Page Language="VB" AutoEventWireup="false" CodeFile="Default.aspx.vb" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <title></title>
    <style>
        #divEditor {
            width: 80%;
            height: 1200px;
            border: 5px solid;
            overflow-x: scroll;
            overflow-y: scroll;
            float: left;
        }

       #tableTiles {
           border-spacing: 0px;
       }

       .tile {
           width: 64px;
           height: 64px;
           border: 2px solid;
       }

       textarea {
           overflow-x: scroll;
           overflow-y: scroll;
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
        <div id="divControls" style="float:right;width:15%">
            <h2>Controls</h2>
            <table>
                <tr>
                    <td><label>Select Tile</label></td>
                    <td>
                        <select id="tileSelector">
                            <option value="1">1</option>
                            <option value="2">2</option>
                            <option value="3">3</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td><label>Tile Type</label></td>
                    <td>
                        <select>
                            <option value="1">Tile</option>
                            <option value="2">Collider</option>
                        </select>
                    </td>
                </tr>
            </table>
            <!-- Hidden inputs for saving the width and height of the current tileset -->
            <input type="hidden" id="hdnWidth" />
            <input type="hidden" id="hdnHeight" />
            <table>
                <tr>
                    <td><label><b>Level Size</b></label></td>
                </tr>
                <tr>
                    <td>&nbsp;</td>
                    <td><label>Width</label></td>
                    <td><input class="numeric" style="width:50px;" type="text" id="txtWidth"/></td>
                    <td><label>Height</label></td>
                    <td><input class="numeric" style="width:50px;" type="text" id="txtHeight"/></td>
                </tr>
                <tr><td><input type="button" value="Generate Area" id="btnGenerate"/></td></tr>
                <tr><td><input type="button" value="Download Tilemap" id="btnDownload"/></td></tr>
            </table>
            <textarea rows="20" cols="20" id="txtTileMap"></textarea>
        </div>
    </form>
</body>
</html>

<script>
    $(document).ready(function () {

        // Restric the inputs with the numeric class to cleanse any non intenger values
        $('.numeric').keyup(function () {
            $(this).prop('value', $(this).prop('value').replace(/[^0-9\.]/g, ''));
        });

        $("#btnGenerate").click(function () {
            // clear out the current table
            $("#tableTiles tr").remove();

            // set loop variables and hidden inputs
            var width = $('#txtWidth').prop('value');
            var height = $('#txtHeight').prop('value');
            $('#hdnWidth').prop('value', width);
            $('#hdnHeight').prop('value', height);

            if (width == '' || height == '') {
                alert("Select a valid width and height");
            }
            else {
                for (var i = 0; i < height; i++) {
                    $('#tableTiles tbody').append('<tr></tr>');
                    for (var j = 0; j < width; j++) {
                        $("#tableTiles tr:last").append('<td><div data-tile-x="' + j + '" data-tile-y="' + i + '" data-tile-type="0" class="tile" id="tile_' + i + '_' + j + '"></div></td>');
                    }
                }
            }
        });
    });

    $('#btnDownload').on('click', function () {
        var width = $('#hdnWidth').prop('value');
        var height = $('#hdnHeight').prop('value');
        var tileMap = '';

        for (var i = 0; i < height; i++) {
            for (var j = 0; j < width; j++) {
                var $tile = $('#tile_' + i + '_' + j);
                tileMap += $tile.attr('data-tile-type');
            }
            tileMap += '\n';
        }

        $('#txtTileMap').prop('value', tileMap);
    });

    // Click on dynamically created divs to set their background image sprites
    $(document).on('click', '.tile', function (event) {
        var tileID = $('#tileSelector').val();
        if (event.shiftKey) {
            $(this).css('background', '');
            $(this).attr('data-tile-type', '0');
        } else {
            switch (tileID) {
                case '1':
                    $(this).css('background', 'url("images/tileset_platforms.png") 0 0');
                    $(this).attr('data-tile-type', '1');
                    break;
                case '2':
                    $(this).css('background', 'url("images/tileset_platforms.png") -74px 0');
                    $(this).attr('data-tile-type', '2');
                    break;
                case '3':
                    $(this).css('background', 'url("images/tileset_platforms.png") -148px 0px');
                    $(this).attr('data-tile-type', '3');
                    break;
            }
        }
    });
</script>