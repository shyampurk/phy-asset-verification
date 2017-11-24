self.onInit = function() {

    self.ctx.labelPosition = self.ctx.settings.labelPosition || 'left';
    if (self.ctx.datasources.length > 0) {
        var tbDatasource = self.ctx.datasources[0];
        var dataKey = tbDatasource.dataKeys[0];
        var datasourceId = 'tbDatasource' + dataKey.label;
        
        self.ctx.$container.append(
            "<div id='" + datasourceId +
            "' class='tbDatasource-container'></div>"
        );
        
        self.ctx.datasourceContainer = $('#' + datasourceId,
            self.ctx.$container);
        var tableId = 'table' + 0;
        self.ctx.datasourceContainer.append(
            "<table id='" + tableId +
            "' class='tbDatasource-table'><col width='30%'><col width='70%'></table>"
        );
        var table = $('#' + tableId, self.ctx.$container);
        if (self.ctx.labelPosition === 'top') {
            table.css('text-align', 'left');
        }
        
        if (tbDatasource.dataKeys.length > 0) {
            //var dataKey = tbDatasource.dataKeys[0];
            var labelCellId = 'labelCell' + dataKey.label;
            var cellId = 'cell' + dataKey.label;
            if (self.ctx.labelPosition === 'left') {
                table.append(
                    "<tr><td id='" + "macid" +"'>" +
                   dataKey.label+
                    "</td></tr><tr><td class='tbDatasource-data-key' id='" + labelCellId +"'>" +
                   "Last Seen" +
                    "</td><td class='tbDatasource-value' id='" +
                    cellId +
                    "'></td></tr>");
            } else {
                table.append(
                    "<tr style='vertical-align: bottom;'><td id='" + "macid" +"'>" +
                   dataKey.label +
                    "</td></tr><tr><td class='tbDatasource-data-key' id='" + labelCellId +"'>" +
                    "Last Seen" +
                    "</td></tr><tr><td class='tbDatasource-value' id='" +
                    cellId +
                    "'></td></tr>");
            }
            self.ctx.labelCell = $('#' + labelCellId, table);
            self.ctx.valueCell = $('#' + cellId, table);
            self.ctx.valueCell.html(0 + ' ' + self.ctx.units);
            
         var n = moment().format("DD/MM/YYYY, hh:mm:ss A");
         self.ctx.valueCell.html(n);
        }
    }
    
    $.fn.textWidth = function(){
        var html_org = $(this).html();
        var html_calc = '<span>' + html_org + '</span>';
        $(this).html(html_calc);
        var width = $(this).find('span:first').width();
        $(this).html(html_org);
        return width;
    };    
    
    self.onResize();
};

self.onDataUpdated = function() {
        var tbDatasource = self.ctx.datasources[0];
        var dataKey = tbDatasource.dataKeys[0];
        var cellData = self.ctx.data[0];
    if (self.ctx.valueCell && self.ctx.data.length > 0) {

        if (cellData.data.length > 0) {
            var tvPair = cellData.data[cellData.data.length - 1];
            var value = tvPair[1];
            
            var datasourceId = '#tbDatasource' + dataKey.label;
            console.log("backup: ",tvPair, value, datasourceId);
            var now='#cell' + dataKey.label; 
            var n = moment().format("DD/MM/YYYY, hh:mm:ss A");
            if ( value === "0" )
            {
                $(datasourceId).css('background-color','red');
                console.log("on if",datasourceId,$(datasourceId).css("background-color"));
                $(".tb-widget").css('padding', '0px');
            }
            else
            {
                
                $(datasourceId).css('background-color','green');
                console.log("on else",datasourceId,$(datasourceId).css("background-color"));
                 $(now).html(n);
                $(".tb-widget").css('padding', '0px');
            }
            
            var targetWidth;
            var minDelta;
            if (self.ctx.labelPosition === 'left') {
                targetWidth = self.ctx.datasourceContainer.width() - self.ctx.labelCell.width();
                minDelta = self.ctx.width/16 + self.ctx.padding;
            } else {
                targetWidth = self.ctx.datasourceContainer.width();
                minDelta = self.ctx.padding;
            }
            var delta = targetWidth - self.ctx.valueCell.textWidth();
            var fontSize = self.ctx.valueFontSize;
            if (targetWidth > minDelta) {
                while (delta < minDelta && fontSize > 6) {
                    fontSize--;
                    self.ctx.valueCell.css('font-size', fontSize+'px');
                    delta = targetWidth - self.ctx.valueCell.textWidth();
                }
            }
        }
    }    
};

self.onResize = function() {
    var labelFontSize;
    if (self.ctx.labelPosition === 'top') {
        self.ctx.padding = self.ctx.height/50;
        labelFontSize = self.ctx.height/2;
        self.ctx.valueFontSize = self.ctx.height/2;
    } else {
        self.ctx.padding = self.ctx.width/50;
        labelFontSize = self.ctx.height/2.5;
        self.ctx.valueFontSize = self.ctx.height/2;
        if (self.ctx.width/self.ctx.height <= 2.7) {
            labelFontSize = self.ctx.width/7;
            self.ctx.valueFontSize = self.ctx.width/6;
        }
    }
    self.ctx.padding = Math.min(12, self.ctx.padding);
    
    if (self.ctx.labelCell) {
        self.ctx.labelCell.css('font-size', '20'+'px');
        self.ctx.labelCell.css('padding', self.ctx.padding+'px');
    }
    if (self.ctx.valueCell) {
        self.ctx.valueCell.css('font-size', '20'+'px');
        self.ctx.valueCell.css('padding', self.ctx.padding+'px');
    }    
};

self.typeParameters = function() {
    return {
        maxDatasources: 1,
        maxDataKeys: 1
    };
};

self.onDestroy = function() {
};

