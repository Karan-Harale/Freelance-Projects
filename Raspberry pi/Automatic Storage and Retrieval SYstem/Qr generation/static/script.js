// Add your custom JavaScript code here
document.addEventListener('DOMContentLoaded', function() {
    // Add event listener for color input change
    document.getElementById('vehicleColor').addEventListener('input', function() {
        document.getElementById('colorPreview').style.backgroundColor = this.value;
    });

    // Add event listener for add remark button
    document.getElementById('addRemarkBtn').addEventListener('click', function() {
        var remarksContainer = document.getElementById('remarksContainer');
        var input = document.createElement('input');
        input.type = 'text';
        input.name = 'remarks';
        input.placeholder = 'Enter remarks';
        remarksContainer.appendChild(input);
    });
});
