const pages = [...document.querySelectorAll('.page')];
const labNumber = document.body.dataset.lab;

function navigate(focus = true) {
    const requested = location.hash.slice(1);
    const current = pages.find(p => p.id === requested) || pages[0];
    pages.forEach(p => p.hidden = p !== current);
    document.querySelectorAll('[data-nav]').forEach(link => {
        const selected = link.dataset.nav === current.id;
        link.classList.toggle('active', selected);
        if (selected) link.setAttribute('aria-current', 'page');
        else link.removeAttribute('aria-current');
    });
    const heading = current.querySelector('h1,h2').textContent.replace(/^\d+\.\s*/, '');
    const crumb = document.getElementById('crumb');
    if (labNumber) {
        const section = current === pages[0] ? '' : ' / ' + heading;
        crumb.textContent = 'Основи програмування / Лабораторна робота №' + labNumber + section;
        document.title = 'Лабораторна №' + labNumber + ' - ' + heading;
    } else {
        crumb.textContent = 'Основи програмування / Головна';
    }
    if (focus) {
        current.focus({ preventScroll: true });
        // на телефоні меню стоїть над текстом, тому показуємо сам розділ
        if (window.innerWidth <= 600) current.scrollIntoView();
        else window.scrollTo(0, 0);
    }
}

window.addEventListener('hashchange', () => navigate());
navigate(false);
