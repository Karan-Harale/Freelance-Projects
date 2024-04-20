document.addEventListener('DOMContentLoaded', function () {
    const addRemarkBtn = document.getElementById('addRemarkBtn');
    const remarksContainer = document.getElementById('remarksContainer');
    const form = document.getElementById('vehicleChecklist');

    addRemarkBtn.addEventListener('click', function () {
        const remarksInput = document.getElementById('remarks');
        const remarkText = remarksInput.value.trim();
        if (remarkText !== '') {
            addRemark(remarkText);
            remarksInput.value = '';
        }
    });

    form.addEventListener('submit', function (event) {
        event.preventDefault();
        // Handle form submission here
        console.log('Form submitted!');
    });

    function addRemark(remarkText) {
        const remarkItem = document.createElement('div');
        remarkItem.classList.add('remark-item');

        const remarkCheckbox = document.createElement('input');
        remarkCheckbox.type = 'checkbox';
        remarkCheckbox.classList.add('remark-checkbox');
        remarkItem.appendChild(remarkCheckbox);

        const remarkLabel = document.createElement('label');
        remarkLabel.textContent = remarkText;
        remarkItem.appendChild(remarkLabel);

        remarksContainer.appendChild(remarkItem);
    }
    
    const vehicleColorInput = document.getElementById('vehicleColor');
    const colorPreview = document.getElementById('colorPreview');

    // Add event listener to vehicle color input
    vehicleColorInput.addEventListener('input', function () {
        const selectedColor = vehicleColorInput.value;
        colorPreview.style.backgroundColor = selectedColor;
    });
});
