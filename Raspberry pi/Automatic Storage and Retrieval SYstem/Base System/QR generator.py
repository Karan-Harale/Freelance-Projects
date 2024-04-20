import qrcode
from openpyxl import Workbook
from openpyxl.drawing.image import Image

# Function to generate QR code and save it
def generate_qr_code(data, filename, box_size=10):
    qr = qrcode.QRCode(
        version=1,
        error_correction=qrcode.constants.ERROR_CORRECT_L,
        box_size=box_size,  # Adjust the box_size parameter here
        border=4,
    )
    qr.add_data(data)
    qr.make(fit=True)
    img = qr.make_image(fill_color="black", back_color="white")
    img.save(filename)

# Function to save data and QR code filename to Excel file
def save_to_excel(data, qr_filename, excel_filename):
    wb = Workbook()
    ws = wb.active
    ws.append(["QR Code Filename", "Data", "QR Code"])
    ws.append([])
    ws.append([qr_filename, data])
    img = Image(qr_filename)
    ws.add_image(img, 'C3')  # Adjust the cell position as needed
    wb.save(excel_filename)

# Example usage
vehicle_type = "Vehicle_Type and Model "
vehicle_CheckSheetlink = "/Vehicle_CheckSheet"

data = f"{vehicle_type} {vehicle_CheckSheetlink}"

qr_filename = "Vehicle_tempref_Number.png"

excel_directory = "Automatic Storage and Retrieval SYstem"
excel_filename = "vehicle_Database.xlsx"

excel_full_path = excel_directory + excel_filename

# Set the desired size of the QR code (adjust box_size parameter)
qr_size = 5  # Example size (adjust as needed)

generate_qr_code(data, qr_filename, box_size=qr_size)
save_to_excel(data, qr_filename, excel_filename)
