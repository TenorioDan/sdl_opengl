<%@ Page Language="VB" AutoEventWireup="false" CodeFile="Default.aspx.vb" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <title></title>
    <style>
        #divEditor {
            width: 60%;
            height: 1200px;
            border: 5px solid;
            overflow: scroll;
            float: left;
        }

        /*#divTileSelect {
            width: 350px;
            height: 300px;
            overflow: scroll;
        }*/

       #tableTiles {
           border-spacing: 0px;
       }

       .tile, .tileSelect {
           width: 64px;
           height: 64px;
           border: 2px solid;
       }

       textarea {
           overflow-x: scroll;
           overflow-y: scroll;
       }

       .tileSelect[data-tile-type="1"]  { background: url(images/tileset_platforms.png) 0 0 }
       .tileSelect[data-tile-type="2"]  { background: url(images/tileset_platforms.png) -74px   0 }
       .tileSelect[data-tile-type="3"]  { background: url(images/tileset_platforms.png) -148px  0 }
       .tileSelect[data-tile-type="4"]  { background: url(images/tileset_platforms.png)  0     -74px }
       .tileSelect[data-tile-type="5"]  { background: url(images/tileset_platforms.png) -74px  -74px }
       .tileSelect[data-tile-type="6"]  { background: url(images/tileset_platforms.png) -148px -74px }
       .tileSelect[data-tile-type="7"]  { background: url(images/tileset_platforms.png) -222px -74px }
       .tileSelect[data-tile-type="8"]  { background: url(images/tileset_platforms.png) -296px -74px }
       .tileSelect[data-tile-type="9"]  { background: url(images/tileset_platforms.png) -370px -74px }
       .tileSelect[data-tile-type="10"] { background: url(images/tileset_platforms.png) -444px -74px }
       .tileSelect[data-tile-type="11"] { background: url(images/tileset_platforms.png) -518px -74px }
       .tileSelect[data-tile-type="12"] { background: url(images/tileset_platforms.png) -592px -74px }

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
        <input type="hidden" id="hdnTileSelected" />
        <div id="divControls" style="float:right;width:35%">
            <h2>Controls</h2>
            <table>
                <tr>
                    <td><label>Select Tile</label></td>
                    <td>
                        <div id="divTileSelect">
                            <table id="tableTileSelect">
                                <tbody>
                                    <tr>
                                        <td><div class="tileSelect" data-tile-type="1"></div></td>
                                        <td><div class="tileSelect" data-tile-type="2"></div></td>
                                        <td><div class="tileSelect" data-tile-type="3"></div></td>
                                        <td><div class="tileSelect" data-tile-type="4"></div></td>
                                    </tr>
                                    <tr>
                                        <td><div class="tileSelect" data-tile-type="5"></div></td>
                                        <td><div class="tileSelect" data-tile-type="6"></div></td>
                                        <td><div class="tileSelect" data-tile-type="7"></div></td>
                                        <td><div class="tileSelect" data-tile-type="8"></div></td>
                                    </tr>
                                    <tr>
                                        <td><div class="tileSelect" data-tile-type="9"></div></td>
                                        <td><div class="tileSelect" data-tile-type="10"></div></td>
                                        <td><div class="tileSelect" data-tile-type="11"></div></td>
                                        <td><div class="tileSelect" data-tile-type="12"></div></td>
                                    </tr>
                                </tbody>
                            </table>
                        </div>
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
            <label>Tilemap</label><br />
            <textarea rows="20" cols="20" id="txtTileMap"></textarea> <br />
            <label>Colliders</label><br />
            <textarea rows="20" cols="20" id="txtColliderMap"></textarea>
        </div>
    </form>
</body>
</html>

<script>
    $(document).ready(function () {

        $('#tileSelector option[value="1"').attr('background', 'url(images/tileset_platforms.png) 0 0')

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

    $('.tileSelect').on('click', function () {

    });

    $('#btnDownload').on('click', function () {
        var width = $('#hdnWidth').prop('value');
        var height = $('#hdnHeight').prop('value');
        var tileMap = '';

        for (var i = 0; i < height; i++) {
            for (var j = 0; j < width; j++) {
                var $tile = $('#tile_' + i + '_' + j);
                tileMap += $tile.attr('data-tile-type') + ' ';
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
            var img = 'url("images/tileset_platforms.png") ';
            var dataattr = 'data-tile-type';
            switch (tileID) {
                case '1':
                    $(this).css('background', img + '0 0');
                    $(this).attr(dataattr, '1');
                    break;
                case '2':
                    $(this).css('background', img + '-74px 0');
                    $(this).attr(dataattr, '2');
                    break;
                case '3':
                    $(this).css('background', img + '-148px 0px');
                    $(this).attr(dataattr, '3');
                    break;
                case '4':
                    $(this).css('background', img + '0 -74px')
                    $(this).attr(dataattr, '4');
                    break;
                case '5':
                    $(this).css('background', img + '-74px -74px')
                    $(this).attr(dataattr, '5');
                    break;
                case '6':
                    $(this).css('background', img + '-148px -74px')
                    $(this).attr(dataattr, '6');
                    break;
                case '7':
                    $(this).css('background', img + '-222px -74px')
                    $(this).attr(dataattr, '7');
                    break;
                case '8':
                    $(this).css('background', img + '-296px -74px')
                    $(this).attr(dataattr, '8');
                    break;
                case '9':
                    $(this).css('background', img + '-370px -74px')
                    $(this).attr(dataattr, '9');
                    break;
                case '10':
                    $(this).css('background', img + '-444px -74px')
                    $(this).attr(dataattr, '10');
                    break;
                case '11':
                    $(this).css('background', img + '-518px -74px')
                    $(this).attr(dataattr, '11');
                    break;
                case '12':
                    $(this).css('background', img + '-592px -74px')
                    $(this).attr(dataattr, '11');
                    break;
            }
        }
    });
</script>