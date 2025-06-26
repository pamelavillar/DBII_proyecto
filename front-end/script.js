let camposGlobales = [];
let tiposGlobales = [];

// Función que convierte el header a campos de búsqueda
function generarCamposBusqueda(headerLine) {
    const searchArea = document.getElementById("search-area");
    searchArea.innerHTML = ""; // Limpiar anterior

    const campos = headerLine.split(",");

    // Inicializar select de ordenamiento
    const ordenarPorSelect = document.getElementById("ordenar-por");
    ordenarPorSelect.innerHTML = '<option value="">-- Selecciona atributo --</option>';

    // Inicializar checkboxes de columnas
    const columnasDiv = document.getElementById("columnas-checkboxes");
    columnasDiv.innerHTML = "";

    camposGlobales = [];
    let tiposGlobales = [];
    campos.forEach(campo => {
      const [nombre, tipo, tam] = campo.split(":");
      camposGlobales.push(nombre);
      tiposGlobales.push(tipo);

      // Agregar opción de ordenamiento
      const option = document.createElement("option");
      option.value = nombre;
      option.textContent = nombre;
      ordenarPorSelect.appendChild(option);

      // Agregar checkbox de selección de columna
      const checkbox = document.createElement("input");
      checkbox.type = "checkbox";
      checkbox.name = "columnas";
      checkbox.value = nombre;
      checkbox.checked = true;

      const labelCheckbox = document.createElement("label");
      labelCheckbox.style.display = "10px";
      labelCheckbox.appendChild(checkbox);
      labelCheckbox.appendChild(document.createTextNode(nombre));

      columnasDiv.appendChild(labelCheckbox);

      const div = document.createElement("div");
      div.classList.add("input-group");

      const label = document.createElement("label");
      label.textContent = `${nombre}:`;

      const input = document.createElement("input");
      input.type = "text";
      input.name = nombre;
      input.placeholder = "Ej: Maria,Jose o 18-30 o 18>";

      div.appendChild(label);
      div.appendChild(input);

      searchArea.appendChild(div);
    });
}

// Leer datos.txt y generar campos
function cargarHeaderDesdeTxt() {
    fetch('datos.txt')
    .then(res => res.text())
    .then(text => {
        const lines = text.trim().split("\n");
        if (lines.length > 0) {
        generarCamposBusqueda(lines[0]);
        } else {
        alert("El archivo está vacío.");
        }
    })
    .catch(err => {
        console.error("Error cargando archivo:", err);
    });
}

// Ejecutar al cargar la página
window.onload = cargarHeaderDesdeTxt;

function guardarConfiguracion() {
  const configuracion = {
    platos: document.getElementById('platos').value,
    pistas: document.getElementById('pistas').value,
    sectores: document.getElementById('sectores').value,
    capacidad: document.getElementById('capacidad').value
  };

  fetch("http://localhost:8080/configurar", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(configuracion)
  })
    .then(res => res.text())
    .then(msg => {
      alert(msg);
    })
    .catch(err => {
      console.error("Error al configurar:", err);
    });
}


function buscarRegistros() {
  const campos_bus = [];
  const valores_bus = [];
  camposGlobales.forEach(attr => {
    const input = document.querySelector(`input[name='${attr}']`);
    if (input && input.value.trim()) {
      campos_bus.push(attr);
      valores_bus.push(input.value.trim());
    }
  });

  const operador = document.getElementById("operador-logico").value;
  const ordenarPor = document.getElementById("ordenar-por").value;
  const direccion = document.getElementById("orden-direccion").value;

  const columnas = Array.from(document.querySelectorAll("input[name='columnas']:checked"))
    .map(cb => cb.value);

  const payload = {
    campos_bus,
    valores_bus,
    operador,
    ordenar: {
      atributo: ordenarPor,
      direccion
    },
    columnas,
    tipos: tiposGlobales,
    nombres: camposGlobales
  };

  console.log("Enviando filtros:", payload);


  fetch('http://localhost:8080/buscar', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  })
  .then(res => res.json())
  .then(data => {
    console.log("Respuesta:", data);
    mostrarResultados(data);
  })
  .catch(err => {
    console.error("Error al enviar datos:", err);
  });
}
    


function mostrarResultados(data) {
  const columnasSeleccionadas = Array.from(document.querySelectorAll("input[name='columnas']:checked"))
    .map(cb => cb.value);

  const thead = document.querySelector("#resultados thead");
  const tbody = document.querySelector("#resultados tbody");

  // Limpiar tabla anterior
  thead.innerHTML = "";
  tbody.innerHTML = "";

  // Cabeceras
  const headerRow = document.createElement("tr");
  columnasSeleccionadas.forEach(col => {
    const th = document.createElement("th");
    th.textContent = col;
    headerRow.appendChild(th);
  });
  thead.appendChild(headerRow);

  // Filas de datos
  data.forEach(fila => {
    const row = document.createElement("tr");
    fila.forEach(celda => {
      const td = document.createElement("td");
      td.textContent = celda;
      row.appendChild(td);
    });
    tbody.appendChild(row);
  });
}
